#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <iomanip> // Mantido para std::setw e std::setfill, se necessário em cout.
#include <cmath>   // Para round
#include <cstdio>  // Para sprintf
#include <cstring> // Para strcpy e outros

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
        char buffer[10]; // Buffer para ler "pac", "org", "dst"
        arquivo >> tempoPostagem >> buffer >> idPacote >> buffer >> idOrigem >> buffer >> idDestino;

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
    
    char msgBuffer[100];
    char idBuffer[4];

    if (armazemId == pacote->getDestino()) {
        pacote->setEstado(Pacote::ENTREGUE);
        formatarId(armazemId, 3, idBuffer);
        sprintf(msgBuffer, "entregue em %s", idBuffer);
        registrarLog(evento->getTempo(), pacote->getId(), msgBuffer);
        pacotesAtivos--;
        delete pacote;
    } else {
        armazens[armazemId]->armazenarPacote(pacote);
        char idBufferProx[4];
        formatarId(armazemId, 3, idBuffer);
        formatarId(pacote->getProximoSalto(), 3, idBufferProx);
        sprintf(msgBuffer, "armazenado em %s na secao %s", idBuffer, idBufferProx);
        registrarLog(evento->getTempo(), pacote->getId(), msgBuffer);
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

    // Substituição de std::sort por Insertion Sort
    for (int i = 1; i < numPacotesNaSecao; i++) {
        Pacote* chave = pacotesNaSecao[i];
        int j = i - 1;
        while (j >= 0 && compararPacotes(chave, pacotesNaSecao[j])) {
            pacotesNaSecao[j + 1] = pacotesNaSecao[j];
            j = j - 1;
        }
        pacotesNaSecao[j + 1] = chave;
    }

    int pacotesATransportarCount = (numPacotesNaSecao < capacidadeTransporte) ? numPacotesNaSecao : capacidadeTransporte;
    
    // Substituição de std::vector por array dinâmico
    Pacote** pacotesSelecionados = new Pacote*[pacotesATransportarCount];
    for(int i = 0; i < pacotesATransportarCount; ++i) {
        pacotesSelecionados[i] = pacotesNaSecao[i];
    }
    
    double tempoDaOperacao = relogio + 1.0; 
    PilhaPacotes pilhaTemporaria;
    
    char msgBuffer[100];
    char idOrigemBuffer[4], idDestinoBuffer[4];
    formatarId(origemId, 3, idOrigemBuffer);
    formatarId(destinoId, 3, idDestinoBuffer);

    while(!pilha->vazia()) {
        Pacote* pacoteDoTopo = pilha->desempilhar();
        tempoDaOperacao += custoRemocao;
        
        sprintf(msgBuffer, "removido de %s na secao %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacoteDoTopo->getId(), msgBuffer);
        
        bool transportarEste = false;
        for(int i=0; i<pacotesATransportarCount; ++i) {
            if(pacotesSelecionados[i] == pacoteDoTopo) {
                transportarEste = true;
                break;
            }
        }

        if(transportarEste) {
            pacoteDoTopo->avancarRota();
            pacoteDoTopo->setEstado(Pacote::EM_TRANSITO);
            sprintf(msgBuffer, "em transito de %s para %s", idOrigemBuffer, idDestinoBuffer);
            registrarLog(tempoDaOperacao, pacoteDoTopo->getId(), msgBuffer);
            Evento* chegadaEv = new ChegadaPacoteEvento(tempoDaOperacao + latenciaTransporte, pacoteDoTopo, destinoId);
            escalonador.Inserir(chegadaEv);
        } else {
            pilhaTemporaria.empilhar(pacoteDoTopo);
        }
    }
    
    while(!pilhaTemporaria.vazia()) {
        Pacote* pacoteParaRearmazenar = pilhaTemporaria.desempilhar();
        pilha->empilhar(pacoteParaRearmazenar);
        sprintf(msgBuffer, "rearmazenado em %s na secao %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacoteParaRearmazenar->getId(), msgBuffer);
    }

    Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
    escalonador.Inserir(proximoTransporte);
    
    delete[] pacotesNaSecao;
    delete[] pacotesSelecionados; // Liberar memória do array dinâmico
}

void Simulador::registrarLog(double tempo, int idPacote, const char* mensagem) {
    char logBuffer[256];
    char idPacoteBuffer[4];
    formatarId(idPacote, 3, idPacoteBuffer);
    
    // Formata a string de log final
    sprintf(logBuffer, "%07d pacote %s %s", static_cast<int>(round(tempo)), idPacoteBuffer, mensagem);
    
    logSaida->adicionaString(logBuffer);
}

void Simulador::imprimirLogs() {
    logSaida->imprime();
}

void Simulador::formatarId(int id, int largura, char* buffer) {
    // Usa sprintf para formatar o número com zeros à esquerda no buffer fornecido
    sprintf(buffer, "%0*d", largura, id);
}