#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <limits> // Required for numeric_limits
#include <iomanip> // Required for std::fixed and std::setprecision

// Define constants for time in hours (already defined in .h, but good to have here for clarity)
// #define TEMPO_MANIPULACAO_PACOTE 1.0
// #define TEMPO_TRANSPORTE_ARMAZENS 3.0

using namespace std;

Simulador::Simulador(const char *arquivotxt) : relogio(0.0), pacotesAtivos(0) // Initialize relogio and pacotesAtivos
{
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
            exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
            }
            if (w < 0 || w >= numArmazens)
            {
                cerr << "Conexão inválida: " << w << " para armazém " << i << endl;
                exit(EXIT_FAILURE);
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
    pacotesAtivos = qtdPacotes; // Initialize with total packages to be processed

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

    // Schedule initial transport events for all connections based on the "once a day" rule.
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
    // Loop continues as long as there are events AND active packages remain.
    // If there are no more active packages, but transport events are still scheduled,
    // they might still run, but won't process packages. This condition helps terminate faster.
    while (!escalonador.Vazio() && pacotesAtivos > 0) 
    {
        Evento* proximoEvento = escalonador.Remover();
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
    cout << "Total de pacotes ativos restantes: " << pacotesAtivos << endl; // Should be 0
}

void Simulador::inicializarTransportes() {
    cout << "\n--- Inicializando eventos de transporte periódicos ---" << endl;
    // Schedule initial transport events for all connected pairs of warehouses.
    // These transports will then reschedule themselves.
    // We iterate through all warehouses and their neighbors to schedule these.
    
    // To avoid duplicate events for undirected edges (A->B and B->A),
    // we can enforce an order (e.g., only schedule if i < destino_id).
    for (int i = 0; i < numArmazens; ++i) {
        Lista* vizinhos = rede->getVizinhos(i);
        Lista* currentVizinho = vizinhos;
        while (currentVizinho != nullptr) {
            int destino_id = currentVizinho->valorInteiro;
            if (i < destino_id) { // Ensure each connection (edge) schedules transport only once
                // Schedule the first transport event at a fixed interval from time 0, e.g., TEMPO_TRANSPORTE_ARMAZENS
                // Assuming capacity of 5 packages per transport as a default
                Evento* transport_ev = new TransporteEvento(TEMPO_TRANSPORTE_ARMAZENS, i, destino_id, 5); 
                escalonador.Inserir(transport_ev);
                cout << "  Agendado transporte periódico inicial de " << i << " para " << destino_id << " em T=" << TEMPO_TRANSPORTE_ARMAZENS << endl;
            }
            currentVizinho = currentVizinho->proximo;
        }
    }
}

void Simulador::processarChegadaPacote(ChegadaPacoteEvento* evento) {
    Pacote* pacote = evento->getPacote();
    int armazem_atual_id = evento->getArmazem();

    pacote->setEstadoAtual(Pacote::CHEGOU_ARMAZEM); // Update state to arrived
    cout << "  Chegada do pacote " << pacote->getId() << " ao armazém " << armazem_atual_id << endl;

    // If it's the final destination, register delivery.
    if (pacote->getDestino() == armazem_atual_id) { 
        registrarEntregaPacote(pacote);
    } else {
        // If it's an intermediate warehouse, store the package.
        // It will wait in the warehouse for the next scheduled transport.
        armazens[armazem_atual_id].armazenarPacote(pacote);
        pacote->setEstadoAtual(Pacote::ARMAZENADO); // Update state to stored
        cout << "    Pacote " << pacote->getId() << " armazenado no armazém " << armazem_atual_id << " aguardando transporte." << endl;
    }
}

void Simulador::processarTransporte(TransporteEvento* evento) {
    int origem_id = evento->getOrigem();
    int destino_id = evento->getDestino(); // This is the next hop
    int capacidade_transporte = evento->getCapacidade();

    cout << "  Processando oportunidade de transporte de " << origem_id << " para " << destino_id << " com capacidade " << capacidade_transporte << endl;

    Secao* secao_origem_para_destino = nullptr;
    // Find the section in the origin warehouse that holds packages for 'destino_id' (the next hop)
    for (int i = 0; i < armazens[origem_id].getNumDestnPossiveis(); ++i) { 
        if (armazens[origem_id].getSecao(i)->getIdArmazem() == destino_id) {
            secao_origem_para_destino = armazens[origem_id].getSecao(i);
            break;
        }
    }

    if (secao_origem_para_destino != nullptr) {
        int pacotes_transportados_count = 0;
        // Keep transporting until section is empty or capacity is reached
        while (!secao_origem_para_destino->vazia() && pacotes_transportados_count < capacidade_transporte) {
            Pacote* pacote_a_transportar = secao_origem_para_destino->desempilhar(); // LIFO
            if (pacote_a_transportar != nullptr) {
                cout << "    Pacote " << pacote_a_transportar->getId() << " recuperado do armazém " << origem_id << " para transporte." << endl;
                pacote_a_transportar->setEstadoAtual(Pacote::EM_TRANSITO); // Update state to in transit

                // Advance the package's route (move to the next hop)
                pacote_a_transportar->avancarRota();

                // Calculate arrival time at next location (current time + transport time)
                double tempo_chegada_no_proximo_local = relogio + TEMPO_TRANSPORTE_ARMAZENS;

                // Schedule the next arrival event for this package
                if (pacote_a_transportar->getProximoSalto() == -1) { 
                    // Package is going to its FINAL destination
                    Evento* chegada_final_ev = new ChegadaPacoteEvento(tempo_chegada_no_proximo_local, pacote_a_transportar, pacote_a_transportar->getDestino());
                    escalonador.Inserir(chegada_final_ev);
                    cout << "    Pacote " << pacote_a_transportar->getId() << " a caminho do DESTINO FINAL " << pacote_a_transportar->getDestino() << " (chega em T=" << fixed << setprecision(2) << tempo_chegada_no_proximo_local << ")" << endl;
                } else {
                    // Package is going to an INTERMEDIATE warehouse
                    Evento* chegada_intermediaria_ev = new ChegadaPacoteEvento(tempo_chegada_no_proximo_local, pacote_a_transportar, pacote_a_transportar->getRota()->valorInteiro);
                    escalonador.Inserir(chegada_intermediaria_ev);
                    cout << "    Pacote " << pacote_a_transportar->getId() << " a caminho do armazém intermediário " << pacote_a_transportar->getRota()->valorInteiro << " (chega em T=" << fixed << setprecision(2) << tempo_chegada_no_proximo_local << ")" << endl;
                }
                pacotes_transportados_count++;
            }
        }
    } else {
        cout << "  Nenhuma seção encontrada ou vazia para transporte de " << origem_id << " para " << destino_id << endl;
    }

    // Reschedule this transport event for the next period, ensuring recurring transports.
    // This is relative to the *current* simulation time.
    Evento* proximo_transport_ev = new TransporteEvento(relogio + TEMPO_TRANSPORTE_ARMAZENS, origem_id, destino_id, capacidade_transporte);
    escalonador.Inserir(proximo_transport_ev);
    cout << "  Agendado próxima oportunidade de transporte de " << origem_id << " para " << destino_id << " em T=" << fixed << setprecision(2) << relogio + TEMPO_TRANSPORTE_ARMAZENS << endl;
}

void Simulador::registrarEntregaPacote(Pacote* pacote) {
    cout << "  !!! Pacote " << pacote->getId() << " ENTREGUE no armazém " << pacote->getDestino() << " em T=" << fixed << setprecision(2) << relogio << " !!!" << endl;
    pacote->setEstadoAtual(Pacote::ENTREGUE); // Update state
    pacotesAtivos--; // Decrement the count of active packages

    // TODO: Calculate and store final package statistics (e.g., total time in system,
    // tempo_armazenado, tempo_em_transito). This needs to be done *before* deleting the package.
    // For `tempo_esperado_estadia`, it would be `relogio - pacote->getTempoChegadaOrigem()`.
    
    // For now, just deleting the package.
    delete pacote; 
}

void Simulador::calcularERegistrarRota(Pacote* pacote) {
    // This method is conceptually handled during package creation and initial scheduling.
    // No direct action needed here during simulation execution.
}