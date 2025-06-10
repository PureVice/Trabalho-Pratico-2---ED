#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>

// --- Implementação da Classe Simulador ---

Simulador::Simulador(const char* arquivoEntrada) {
    this->rede = nullptr;
    this->armazens = nullptr;
    this->relogio = 0.0;
    this->pacotesAtivos = 0;
    this->logSaida = new Lista();
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
}

void Simulador::carregarDados(const char* arquivoEntrada) {
    std::ifstream arquivo(arquivoEntrada);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir o arquivo de entrada: " << arquivoEntrada << std::endl;
        exit(EXIT_FAILURE);
    }

    arquivo >> capacidadeTransporte;
    arquivo >> latenciaTransporte;
    arquivo >> intervaloTransportes;
    arquivo >> custoRemocao;

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
    
    int numPacotes;
    arquivo >> numPacotes;
    this->pacotesAtivos = numPacotes;
    for (int i = 0; i < numPacotes; ++i) {
        double tempoPostagem;
        int idPacote, idOrigem, idDestino;
        std::string pac_str, org_str, dst_str; 
        arquivo >> tempoPostagem >> pac_str >> idPacote >> org_str >> idOrigem >> dst_str >> idDestino;

        Pacote* p = new Pacote(idPacote, idOrigem, idDestino, tempoPostagem);
        p->setRota(calculaRota(*rede, idOrigem, idDestino));
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
        // O relógio só avança se o tempo do evento for futuro.
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
    
    if (armazemId == pacote->getDestino()) {
        pacote->setEstado(Pacote::ENTREGUE);
        registrarLog(evento->getTempo(), pacote->getId(), "entregue em " + formatarId(armazemId, 3));
        pacotesAtivos--;
        delete pacote;
    } else {
        armazens[armazemId]->armazenarPacote(pacote);
        registrarLog(evento->getTempo(), pacote->getId(), "armazenado em " + formatarId(armazemId, 3) + " na secao " + formatarId(pacote->getProximoSalto(), 3));
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
    std::vector<Pacote*> pacotesSelecionados(pacotesNaSecao, pacotesNaSecao + pacotesATransportarCount);
    
    double tempoDaOperacao = relogio + 1.0; 
    PilhaPacotes pilhaTemporaria;
    
    while(!pilha->vazia()) {
        Pacote* pacoteDoTopo = pilha->desempilhar();
        tempoDaOperacao += custoRemocao;
        registrarLog(tempoDaOperacao, pacoteDoTopo->getId(), "removido de " + formatarId(origemId, 3) + " na secao " + formatarId(destinoId, 3));
        
        bool transportarEste = false;
        for(size_t i=0; i<pacotesSelecionados.size(); ++i) {
            if(pacotesSelecionados[i] == pacoteDoTopo) {
                transportarEste = true;
                break;
            }
        }
        if(transportarEste) {
            // *** CORREÇÃO DO LOOP INFINITO ESTÁ AQUI ***
            // Avançamos a rota do pacote no momento da saída.
            pacoteDoTopo->avancarRota();
            
            pacoteDoTopo->setEstado(Pacote::EM_TRANSITO);
            registrarLog(tempoDaOperacao, pacoteDoTopo->getId(), "em transito de " + formatarId(origemId, 3) + " para " + formatarId(destinoId, 3));
            Evento* chegadaEv = new ChegadaPacoteEvento(tempoDaOperacao + latenciaTransporte, pacoteDoTopo, destinoId);
            escalonador.Inserir(chegadaEv);
        } else {
            pilhaTemporaria.empilhar(pacoteDoTopo);
        }
    }
    
    while(!pilhaTemporaria.vazia()) {
        Pacote* pacoteParaRearmazenar = pilhaTemporaria.desempilhar();
        pilha->empilhar(pacoteParaRearmazenar);
        registrarLog(tempoDaOperacao, pacoteParaRearmazenar->getId(), "rearmazenado em " + formatarId(origemId, 3) + " na secao " + formatarId(destinoId, 3));
    }

    Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
    escalonador.Inserir(proximoTransporte);
    
    delete[] pacotesNaSecao;
}

void Simulador::registrarLog(double tempo, int idPacote, const std::string& mensagem) {
    std::ostringstream oss;
    oss << std::setw(7) << std::setfill('0') << static_cast<int>(round(tempo)) << " pacote " << formatarId(idPacote, 3) << " " << mensagem;
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