#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <limits> // Required for numeric_limits
#include <iomanip> // Required for std::fixed and std::setprecision

// Define constants for time in hours (already defined in .h, but good to have here for clarity)
// #define TEMPO_MANIPULACAO_PACOTE 1.0
// #define TEMPO_TRANSPORTE_ARMAZENS 3.0

using namespace std;

Simulador::Simulador(const char *arquivotxt) : relogio(0.0) // Initialize relogio to 0.0
{
    // The Escalonador is a member object, so its default constructor is called.
    // If it requires parameters, you'd pass them in the initializer list.
    
    std::ifstream arquivo(arquivotxt);
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << arquivotxt << std::endl;
        exit(EXIT_FAILURE);
    }

    // Read number of warehouses
    arquivo >> numArmazens;
    this->rede = new Rede(numArmazens);
    this->armazens = new Armazem[numArmazens];

    // Build the network (graph)
    for (int i = 0; i < numArmazens; i++)
    {
        int qtdArestas;
        if (!(arquivo >> qtdArestas))
        {
            cerr << "Erro ao ler quantidade de arestas para armazém " << i << endl;
            exit(EXIT_FAILURE); // Exit on error
        }
        this->armazens[i].setId(i);
        this->armazens[i].setNumDestnPossiveis(numArmazens); // Set total possible destinations

        cout << "Armazém " << i << " com " << qtdArestas << " conexões:" << endl;

        for (int j = 0; j < qtdArestas; j++)
        {
            int w;
            if (!(arquivo >> w))
            {
                cerr << "Erro ao ler conexão " << j << " do armazém " << i << endl;
                exit(EXIT_FAILURE); // Exit on error
            }
            if (w < 0 || w >= numArmazens)
            {
                cerr << "Conexão inválida: " << w << " para armazém " << i << endl;
                exit(EXIT_FAILURE); // Exit on error
            }
            cout << "  Conexão com armazém " << w << endl;
            this->rede->InsereAresta(i, w);
            this->rede->InsereAresta(w, i); // Graph is undirected, so add both ways
        }
    }

    // Basic network validation
    if (this->rede->QuantidadeArmazens() != numArmazens)
    {
        cerr << "Erro: Quantidade de armazéns não corresponde!" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "\n=== Teste da Rede ===" << endl;
    cout << "Armazens: " << this->rede->QuantidadeArmazens() << endl;
    cout << "Arestas: " << this->rede->QuantidadeArestas() << endl;

    for (int i = 0; i < numArmazens; i++)
    {
        cout << "Vizinhos do armazem " << i << ": ";
        this->rede->ImprimeVizinhos(i);
    }
    
    // Read and schedule initial packages
    int qtdPacotes;
    arquivo >> qtdPacotes;

    for (int i = 0; i < qtdPacotes; i++)
    {
        int id = i;
        char remetente[100];
        char destinatario[100];
        char tipo;
        int id_armz_orig;
        int id_armz_dest;
        double tempo_chegada;
        
        arquivo >> id_armz_orig >> id_armz_dest >> remetente >> destinatario >> tipo >> tempo_chegada;
        
        Pacote *p = new Pacote(id, remetente, destinatario, tipo, id_armz_orig, id_armz_dest, tempo_chegada);
        
        // Calculate and set route for the package
        int tam_rota = 0; // This will be updated by calculaRota2
        Lista *rota = calculaRota2(*rede, id_armz_orig, id_armz_dest, numArmazens, tam_rota);
        p->setRota(rota);

        cout << "\nPacote " << p->getId() << " lido:" << endl;
        p->imprimePacote();
        cout << "Rota do pacote " << p->getId() << ": ";
        p->imprimeRota();

        // Schedule the initial CHEGADA_PACOTE event for each package
        Evento* ev = new ChegadaPacoteEvento(tempo_chegada, p, id_armz_orig);
        escalonador.Inserir(ev);
    }
    arquivo.close();

    // Schedule initial transport events for all connections
    inicializarTransportes();
}

Simulador::~Simulador()
{
    delete rede; // Libera a memória da rede
    delete[] armazens; // Libera a memória alocada para os armazéns
    // The escalonador's destructor will handle its events.
}

void Simulador::executar()
{
    // Main simulation loop
    while (!escalonador.Vazio())
    {
        Evento* proximoEvento = escalonador.Remover(); // Get the next event
        relogio = proximoEvento->getTempo(); // Advance simulation clock

        cout << "\n--- Processando evento em T=" << fixed << setprecision(2) << relogio << " ---" << endl;

        // Process the event based on its type
        if (proximoEvento->getTipo() == CHEGADA_PACOTE)
        {
            processarChegadaPacote(static_cast<ChegadaPacoteEvento*>(proximoEvento));
        }
        else if (proximoEvento->getTipo() == TRANSPORTE)
        {
            processarTransporte(static_cast<TransporteEvento*>(proximoEvento));
        }
        
        // The event object is deleted by the Escalonador's destructor or when it's processed.
        // For now, let's delete it here after processing to avoid memory leaks for processed events.
        delete proximoEvento; 
    }
    cout << "\nSimulação concluída em T=" << fixed << setprecision(2) << relogio << endl;

    // TODO: Generate final statistics here
}

void Simulador::inicializarTransportes() {
    // For each connection between warehouses, schedule a recurring transport event.
    // The problem states "Uma vez por dia pacotes são transportados de um armazém para o outro".
    // Assuming "dia" means 24 hours for simplicity if no other frequency is given.
    // And "capacidade" is also not explicitly given per connection, assuming a default.
    // For now, let's just schedule a transport event from each warehouse to its direct neighbors.
    // The frequency will be TEMPO_TRANSPORTE_ARMAZENS.

    cout << "\n--- Inicializando eventos de transporte ---" << endl;
    for (int i = 0; i < numArmazens; ++i) {
        Lista* vizinhos = rede->getVizinhos(i);
        Lista* currentVizinho = vizinhos;
        while (currentVizinho != nullptr) {
            int destino_id = currentVizinho->valorInteiro;
            // Schedule the first transport event at TEMPO_TRANSPORTE_ARMAZENS from now
            // Or perhaps at a predefined interval, depending on interpretation.
            // Let's assume daily transports from the beginning of the simulation (relogio = 0)
            // and subsequent transports every TEMPO_TRANSPORTE_ARMAZENS hours.
            
            // For simplicity in this initial setup, we will schedule a transport event from
            // origin 'i' to 'destino_id' at an arbitrary future time, e.g., 24 hours.
            // A more robust simulation would need to manage recurring events properly.
            // For the scope of the problem, a single transport event can trigger the next one.
            
            // This needs to be handled carefully: A transport event will process packages
            // from one origin to one destination.
            // We need to ensure that a transport event is scheduled for each *pair* of connected warehouses.
            
            // To avoid duplicate transport events (A->B and B->A scheduled separately but representing the same trip),
            // we could enforce an order (e.g., i < destino_id).
            if (i < destino_id) { // Only schedule for one direction of the undirected edge
                Evento* transport_ev = new TransporteEvento(TEMPO_TRANSPORTE_ARMAZENS, i, destino_id, 5); // Assuming capacity 5 for now
                escalonador.Inserir(transport_ev);
                cout << "  Agendado transporte de " << i << " para " << destino_id << " em T=" << TEMPO_TRANSPORTE_ARMAZENS << endl;
            }
            currentVizinho = currentVizinho->proximo;
        }
    }
}

void Simulador::processarChegadaPacote(ChegadaPacoteEvento* evento) {
    Pacote* pacote = evento->getPacote();
    int armazem_atual_id = evento->getArmazem();

    cout << "  Chegada do pacote " << pacote->getId() << " ao armazém " << armazem_atual_id << endl;

    // A package arriving at an warehouse should be stored
    armazens[armazem_atual_id].armazenarPacote(pacote);

    // After storing, the package might be ready for the next transport.
    // The next transport event will pick it up.
    // No new event is scheduled immediately after storing, as transports happen "once a day".
}

void Simulador::processarTransporte(TransporteEvento* evento) {
    int origem_id = evento->getOrigem();
    int destino_id = evento->getDestino();
    int capacidade_transporte = evento->getCapacidade();

    cout << "  Processando transporte de " << origem_id << " para " << destino_id << " com capacidade " << capacidade_transporte << endl;

    // Check if there are packages to transport from origin to destination
    // For each possible next destination from 'origem_id', check if there are packages.
    // The problem states: "Uma vez por dia pacotes são transportados de um armazém para o outro, de acordo com a sua rota, com prioridade para os pacotes que estão em trânsito há mais tempo."
    // And "Há um limite do número de pacotes que podem ser transportados por dia entre dois armazéns."

    int pacotes_transportados_count = 0;
    // Iterate through sections of the origin warehouse to find packages destined for 'destino_id'
    // or any other next hop on their route from 'origem_id'.
    
    // The problem statement implies packages are stored in sections based on their *next* destination.
    // So, we need to check the section corresponding to 'destino_id' in 'origem_id' warehouse.

    // Find the section in 'origem_id' warehouse that corresponds to 'destino_id'
    Secao* secao_origem_para_destino = nullptr;
    for (int i = 0; i < numArmazens; ++i) { // Iterate through possible sections in the origin warehouse
        if (armazens[origem_id].getSecao(i)->getIdArmazem() == destino_id) {
            secao_origem_para_destino = armazens[origem_id].getSecao(i);
            break;
        }
    }

    if (secao_origem_para_destino != nullptr) {
        while (!secao_origem_para_destino->vazia() && pacotes_transportados_count < capacidade_transporte) {
            Pacote* pacote_a_transportar = secao_origem_para_destino->desempilhar(); // LIFO
            if (pacote_a_transportar != nullptr) {
                // Advance the package's route (move to the next hop)
                pacote_a_transportar->avancarRota();

                // Schedule arrival at the next destination
                // The time of arrival is current_time + TEMPO_TRANSPORTE_ARMAZENS
                double tempo_chegada_destino = relogio + TEMPO_TRANSPORTE_ARMAZENS;
                
                Evento* chegada_destino_ev;
                // Check if the package has reached its final destination
                if (pacote_a_transportar->getProximoSalto() == -1) { // -1 means final destination
                    chegada_destino_ev = new ChegadaPacoteEvento(tempo_chegada_destino, pacote_a_transportar, pacote_a_transportar->getDestino());
                    cout << "    Pacote " << pacote_a_transportar->getId() << " a caminho do destino final " << pacote_a_transportar->getDestino() << " (chega em T=" << fixed << setprecision(2) << tempo_chegada_destino << ")" << endl;
                } else {
                    chegada_destino_ev = new ChegadaPacoteEvento(tempo_chegada_destino, pacote_a_transportar, pacote_a_transportar->getRota()->valorInteiro);
                    cout << "    Pacote " << pacote_a_transportar->getId() << " a caminho do armazém intermediário " << pacote_a_transportar->getRota()->valorInteiro << " (chega em T=" << fixed << setprecision(2) << tempo_chegada_destino << ")" << endl;
                }
                escalonador.Inserir(chegada_destino_ev);
                pacotes_transportados_count++;
            }
        }
    } else {
        cout << "  Nenhuma seção encontrada para transporte de " << origem_id << " para " << destino_id << endl;
    }

    // Reschedule the next transport event for this pair of warehouses
    // Assuming transports happen periodically.
    Evento* proximo_transport_ev = new TransporteEvento(relogio + TEMPO_TRANSPORTE_ARMAZENS, origem_id, destino_id, capacidade_transporte);
    escalonador.Inserir(proximo_transport_ev);
}


// This method is called when a package reaches its final destination.
void Simulador::registrarEntregaPacote(Pacote* pacote) {
    cout << "  !!! Pacote " << pacote->getId() << " ENTREGUE no armazém " << pacote->getDestino() << " em T=" << fixed << setprecision(2) << relogio << " !!!" << endl;
    // TODO: Update package status to DELIVERED and calculate final metrics.
    // For now, we just print a message.
    delete pacote; // Package delivered, can be deleted from memory
}

void Simulador::calcularERegistrarRota(Pacote* pacote) {
    // This is handled during package creation in the constructor,
    // where calculaRota2 is called and the result is set using pacote->setRota().
    // So this method might not be strictly necessary as a separate step in the simulation loop.
}