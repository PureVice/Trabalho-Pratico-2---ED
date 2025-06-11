#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <cstring>

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
        char buffer[10];
        arquivo >> tempoPostagem >> buffer >> idPacote >> buffer >> idOrigem >> buffer >> idDestino;
        idPacote = i;
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
    bool primeiroPacoteProcessado = false; // NOVO: Flag de controle

    while (!escalonador.Vazio() && pacotesAtivos > 0)
    {
        Evento *proximoEvento = escalonador.Remover();
        
        // NOVO: Bloco para imprimir o timestamp do primeiro evento de pacote
        if (!primeiroPacoteProcessado && proximoEvento->getTipo() == CHEGADA_PACOTE)
        {
            tempoPrimeiroPacote = proximoEvento->getTempo();
            primeiroPacoteProcessado = true;
        }
        if (proximoEvento->getTempo() > relogio)
        {
            relogio = proximoEvento->getTempo();
        }

        if (proximoEvento->getTipo() == CHEGADA_PACOTE)
        {
            processarChegadaPacote(static_cast<ChegadaPacoteEvento *>(proximoEvento));
        }
        else if (proximoEvento->getTipo() == TRANSPORTE)
        {
            // relogio+=tempoPrimeiroPacote;
            processarTransporte(static_cast<TransporteEvento *>(proximoEvento));
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
    if (a->getTempoPostagem() != b->getTempoPostagem()) {
        return a->getTempoPostagem() < b->getTempoPostagem();
    }
    // Critério de desempate: ordena por ID do pacote se os tempos de postagem forem iguais.
    return a->getId() < b->getId();
}

void Simulador::processarTransporte(TransporteEvento* evento) {
        int origemId = evento->getOrigem();
    int destinoId = evento->getDestino();

    // Reagenda o próximo transporte para esta rota imediatamente
    Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
    escalonador.Inserir(proximoTransporte);

    Secao* secao = armazens[origemId]->getSecaoPorDestino(destinoId);
    if (secao == nullptr || secao->estaVazia()) {
        return; // Nenhum pacote para transportar, evento já foi reagendado.
    }

    PilhaPacotes* pilha = secao->getPilha();
    int numPacotesNaSecao = pilha->getTamanho();
    Pacote** pacotesNaSecaoLIFO = pilha->getPacotes(); // Pacotes em ordem LIFO (topo -> fundo)

    // --- Seleção FIFO ---
    // Os pacotes com prioridade são os que estão no fundo da pilha (chegaram primeiro).
    int pacotesATransportarCount = (numPacotesNaSecao < capacidadeTransporte) ? numPacotesNaSecao : capacidadeTransporte;
    
    // pacotesSelecionados conterá os N pacotes com prioridade, já em ordem FIFO.
    Pacote** pacotesSelecionados = new Pacote*[pacotesATransportarCount];
    for (int i = 0; i < pacotesATransportarCount; ++i) {
        // Pega do final do array LIFO para obter a ordem FIFO
        // Ex: Para 3 pacotes e capacidade 2, pega os índices 2 e 1 do array LIFO.
        pacotesSelecionados[i] = pacotesNaSecaoLIFO[numPacotesNaSecao - 1 - i];
    }
    
    double tempoDaOperacao = relogio + tempoPrimeiroPacote; 
    PilhaPacotes pilhaTemporaria;
    
    char msgBuffer[100];
    char idOrigemBuffer[4], idDestinoBuffer[4];
    formatarId(origemId, 3, idOrigemBuffer);
    formatarId(destinoId, 3, idDestinoBuffer);

    // --- FASE 1: Remoção e Particionamento ---
    while (!pilha->vazia()) {
        Pacote* pacoteDoTopo = pilha->desempilhar();
        tempoDaOperacao += custoRemocao;
        
        sprintf(msgBuffer, "removido de %s na secao %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacoteDoTopo->getId(), msgBuffer);
        
        bool transportarEste = false;
        for (int i = 0; i < pacotesATransportarCount; ++i) {
            if (pacotesSelecionados[i] == pacoteDoTopo) {
                transportarEste = true;
                break;
            }
        }

        if (!transportarEste) {
            pilhaTemporaria.empilhar(pacoteDoTopo);
        }
    }
    
    // --- FASE 2: Log de Transporte e Escalonamento ---
    // Itera sobre o array `pacotesSelecionados`, que já está na ordem correta (FIFO).
    for (int i = 0; i < pacotesATransportarCount; ++i) {
        Pacote* pacote = pacotesSelecionados[i]; 
        pacote->avancarRota();
        pacote->setEstado(Pacote::EM_TRANSITO);

        sprintf(msgBuffer, "em transito de %s para %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacote->getId(), msgBuffer);

        Evento* chegadaEv = new ChegadaPacoteEvento(tempoDaOperacao + latenciaTransporte, pacote, destinoId);
        escalonador.Inserir(chegadaEv);
    }

    // --- FASE 3: Rearmazenamento ---
    while (!pilhaTemporaria.vazia()) {
        Pacote* pacoteParaRearmazenar = pilhaTemporaria.desempilhar();
        pilha->empilhar(pacoteParaRearmazenar);
        sprintf(msgBuffer, "rearmazenado em %s na secao %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacoteParaRearmazenar->getId(), msgBuffer);
    }
    
    delete[] pacotesNaSecaoLIFO;
    delete[] pacotesSelecionados;
}

void Simulador::registrarLog(double tempo, int idPacote, const char* mensagem) {
    char logBuffer[256];
    char idPacoteBuffer[4];
    formatarId(idPacote, 3, idPacoteBuffer);
    
    sprintf(logBuffer, "%07d pacote %s %s", static_cast<int>(round(tempo)), idPacoteBuffer, mensagem);
    
    logSaida->adicionaString(logBuffer);
}

void Simulador::imprimirLogs() {
    logSaida->imprime();
}

void Simulador::formatarId(int id, int largura, char* buffer) {
    sprintf(buffer, "%0*d", largura, id);
}