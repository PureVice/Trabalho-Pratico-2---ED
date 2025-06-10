#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <iomanip> // Para formatação da saída (setw, setfill)
#include <sstream> // Para construir as strings de log
#include <algorithm> // Para std::sort

// --- Implementação da Classe Simulador ---

Simulador::Simulador(const char* arquivoEntrada) {
    // Inicialização dos membros
    this->rede = nullptr;
    this->armazens = nullptr;
    this->relogio = 0.0;
    this->pacotesAtivos = 0;
    this->logSaida = new Lista();
    
    // Carrega todos os dados do arquivo de entrada.
    carregarDados(arquivoEntrada);
}

Simulador::~Simulador() {
    delete rede;
    if (armazens != nullptr) {
        for (int i = 0; i < numArmazens; ++i) {
            delete armazens[i];
        }
        delete[] armazens;
    }
    delete logSaida;
    // O escalonador e os pacotes restantes são limpos em seus próprios destrutores.
}

void Simulador::carregarDados(const char* arquivoEntrada) {
    std::ifstream arquivo(arquivoEntrada);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir o arquivo de entrada: " << arquivoEntrada << std::endl;
        exit(EXIT_FAILURE);
    }

    // 1. Leitura dos parâmetros da simulação
    arquivo >> capacidadeTransporte;
    arquivo >> latenciaTransporte;
    arquivo >> intervaloTransportes;
    arquivo >> custoRemocao;

    // 2. Leitura da topologia da rede
    arquivo >> numArmazens;
    this->rede = new Rede(numArmazens);
    this->armazens = new Armazem*[numArmazens];

    for (int i = 0; i < numArmazens; ++i) {
        armazens[i] = new Armazem(i, numArmazens);
        for (int j = 0; j < numArmazens; ++j) {
            int conectado;
            arquivo >> conectado;
            if (conectado == 1) {
                rede->InsereAresta(i, j);
            }
        }
    }
    
    // 3. Leitura dos pacotes
    int numPacotes;
    arquivo >> numPacotes;
    this->pacotesAtivos = numPacotes;

    for (int i = 0; i < numPacotes; ++i) {
        double tempoPostagem;
        int idPacote, idOrigem, idDestino;
        std::string pac_str, org_str, dst_str; 
        arquivo >> tempoPostagem >> pac_str >> idPacote >> org_str >> idOrigem >> dst_str >> idDestino;

        Pacote* p = new Pacote(idPacote, idOrigem, idDestino, tempoPostagem);
        
        Lista* rota = calculaRota(*rede, idOrigem, idDestino);
        p->setRota(rota);

        p->setEstado(Pacote::CHEGADA_AGENDADA);
        Evento* ev = new ChegadaPacoteEvento(tempoPostagem, p, idOrigem);
        escalonador.Inserir(ev);
    }

    arquivo.close();
}


void Simulador::executar() {
    inicializarTransportes();

    while (!escalonador.Vazio() && pacotesAtivos > 0) {
        Evento* proximoEvento = escalonador.Remover();
        
        if (proximoEvento->getTempo() > relogio) {
            relogio = proximoEvento->getTempo();
        }

        if (proximoEvento->getTipo() == CHEGADA_PACOTE) {
            processarChegadaPacote(static_cast<ChegadaPacoteEvento*>(proximoEvento));
        } else if (proximoEvento->getTipo() == TRANSPORTE) {
            processarTransporte(static_cast<TransporteEvento*>(proximoEvento));
        }
        
        delete proximoEvento;
    }

    imprimirLogs();
}

void Simulador::inicializarTransportes() {
    for (int i = 0; i < numArmazens; ++i) {
        Lista* vizinhos = rede->getVizinhos(i);
        Celula* atual = vizinhos->getInicio();
        while(atual != nullptr) {
            int vizinhoId = atual->valorInteiro;
            if (i < vizinhoId) {
                Evento* ev = new TransporteEvento(intervaloTransportes, i, vizinhoId);
                escalonador.Inserir(ev);
                Evento* ev_rev = new TransporteEvento(intervaloTransportes, vizinhoId, i);
                escalonador.Inserir(ev_rev);
            }
            atual = atual->proximo;
        }
    }
}

void Simulador::processarChegadaPacote(ChegadaPacoteEvento* evento) {
    Pacote* pacote = evento->getPacote();
    int armazemId = evento->getArmazemChegada();
    
    // ***************************************************************
    // ** CORREÇÃO 1: A lógica de avançar a rota foi REMOVIDA daqui. **
    // ***************************************************************

    if (armazemId == pacote->getDestino()) {
        pacote->setEstado(Pacote::ENTREGUE);
        registrarLog(relogio, pacote->getId(), "entregue em " + formatarId(armazemId, 3));
        pacotesAtivos--;
        delete pacote;
    } else {
        armazens[armazemId]->armazenarPacote(pacote);
        registrarLog(relogio, pacote->getId(), "armazenado em " + formatarId(armazemId, 3) + " na secao " + formatarId(pacote->getProximoSalto(), 3));
    }
}

bool compararPacotes(Pacote* a, Pacote* b) {
    return a->getTempoPostagem() < b->getTempoPostagem();
}

void Simulador::processarTransporte(TransporteEvento* evento) {
    int origemId = evento->getOrigem();
    int destinoId = evento->getDestino();

    Secao* secao = armazens[origemId]->getSecaoPorDestino(destinoId);
    if (secao == nullptr || secao->estaVazia()) {
        Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
        escalonador.Inserir(proximoTransporte);
        return;
    }

    PilhaPacotes* pilha = secao->getPilha();
    int numPacotesNaSecao = pilha->getTamanho();
    Pacote** pacotesNaSecao = pilha->getPacotes();
    
    std::sort(pacotesNaSecao, pacotesNaSecao + numPacotesNaSecao, compararPacotes);

    int pacotesATransportarCount = std::min(numPacotesNaSecao, capacidadeTransporte);
    Pacote** pacotesSelecionados = new Pacote*[pacotesATransportarCount];
    for(int i=0; i<pacotesATransportarCount; ++i) {
        pacotesSelecionados[i] = pacotesNaSecao[i];
    }
    
    double tempoAtualOperacao = relogio;
    PilhaPacotes pilhaTemporaria;

    while(!pilha->vazia()) {
        Pacote* pacoteDoTopo = pilha->desempilhar();
        bool transportarEste = false;
        for(int i=0; i<pacotesATransportarCount; ++i) {
            if(pacotesSelecionados[i] == pacoteDoTopo) {
                transportarEste = true;
                break;
            }
        }

        if(transportarEste) {
            registrarLog(tempoAtualOperacao, pacoteDoTopo->getId(), "removido de " + formatarId(origemId, 3) + " na secao " + formatarId(destinoId, 3));
            
            // *************************************************************************
            // ** CORREÇÃO 2: A rota é avançada AQUI, no momento da SAÍDA. **
            // *************************************************************************
            pacoteDoTopo->avancarRota();
            
            double tempoChegada = tempoAtualOperacao + latenciaTransporte;
            pacoteDoTopo->setEstado(Pacote::EM_TRANSITO);
            registrarLog(tempoAtualOperacao, pacoteDoTopo->getId(), "em transito de " + formatarId(origemId, 3) + " para " + formatarId(destinoId, 3));

            Evento* chegadaEv = new ChegadaPacoteEvento(tempoChegada, pacoteDoTopo, destinoId);
            escalonador.Inserir(chegadaEv);

        } else {
            tempoAtualOperacao += custoRemocao;
            pilhaTemporaria.empilhar(pacoteDoTopo);
        }
    }

    while(!pilhaTemporaria.vazia()) {
        Pacote* pacoteParaRearmazenar = pilhaTemporaria.desempilhar();
        pilha->empilhar(pacoteParaRearmazenar);
        registrarLog(tempoAtualOperacao, pacoteParaRearmazenar->getId(), "rearmazenado em " + formatarId(origemId, 3) + " na secao " + formatarId(destinoId, 3));
    }

    Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
    escalonador.Inserir(proximoTransporte);
    
    delete[] pacotesNaSecao;
    delete[] pacotesSelecionados;
}

void Simulador::registrarLog(double tempo, int idPacote, const std::string& mensagem) {
    std::ostringstream oss;
    oss << std::setw(7) << std::setfill('0') << static_cast<int>(tempo)
        << " pacote " << formatarId(idPacote, 3) << " " << mensagem;
    logSaida->adicionaString(oss.str());
}

void Simulador::imprimirLogs() {
    logSaida->imprime();
}

std::string Simulador::formatarId(int id, int largura) {
    std::ostringstream oss;
    oss << std::setw(largura) << std::setfill('0') << id;
    return oss.str();
}