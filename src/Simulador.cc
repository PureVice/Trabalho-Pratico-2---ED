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

    // --- DEBUG ---
    std::cerr << "[DEBUG] Parametros Carregados: capacidadeTransporte=" << capacidadeTransporte 
              << ", latencia=" << latenciaTransporte << ", intervalo=" << intervaloTransportes 
              << ", custoRemocao=" << custoRemocao << std::endl;
    // --- FIM DEBUG ---

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
        
        // --- DEBUG ---
        std::cerr << "[DEBUG] Pacote Lido do Arquivo: ID=" << idPacote << ", Origem=" << idOrigem 
                  << ", Destino=" << idDestino << ", Tempo=" << tempoPostagem << std::endl;
        // --- FIM DEBUG ---

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

        // --- DEBUG ---
        std::cerr << "\n[DEBUG] === Processando Evento === Relogio: " << relogio << " === Pacotes Ativos: " << pacotesAtivos << " ===" << std::endl;
        if (proximoEvento->getTipo() == CHEGADA_PACOTE) {
            ChegadaPacoteEvento* cpe = static_cast<ChegadaPacoteEvento*>(proximoEvento);
            std::cerr << "[DEBUG]   - Evento de CHEGADA_PACOTE ID=" << cpe->getPacote()->getId() << " no Armazem=" << cpe->getArmazemChegada() << " em T=" << cpe->getTempo() << std::endl;
        } else if (proximoEvento->getTipo() == TRANSPORTE) {
            TransporteEvento* te = static_cast<TransporteEvento*>(proximoEvento);
            std::cerr << "[DEBUG]   - Evento de TRANSPORTE de " << te->getOrigem() << " para " << te->getDestino() << " em T=" << te->getTempo() << std::endl;
        }
        // --- FIM DEBUG ---

        if (proximoEvento->getTipo() == CHEGADA_PACOTE) {
            processarChegadaPacote(static_cast<ChegadaPacoteEvento*>(proximoEvento));
        } else if (proximoEvento->getTipo() == TRANSPORTE) {
            processarTransporte(static_cast<TransporteEvento*>(proximoEvento));
        }
        delete proximoEvento;
    }
    std::cerr << "\n[DEBUG] === Fim da Simulacao === Nao ha mais eventos ou pacotes." << std::endl;
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

    Secao* secao = armazens[origemId]->getSecaoPorDestino(destinoId);
    if (secao == nullptr || secao->estaVazia()) {
        Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
        escalonador.Inserir(proximoTransporte);
        return;
    }

    // --- DEBUG ---
    std::cerr << "[DEBUG]   - Status da Secao " << origemId << "->" << destinoId << " antes do transporte:";
    secao->getPilha()->imprimePilha(std::cerr);
    std::cerr << " [BASE]" << std::endl;
    // --- FIM DEBUG ---

    PilhaPacotes* pilha = secao->getPilha();
    int numPacotesNaSecao = pilha->getTamanho();
    Pacote** pacotesNaSecao = pilha->getPacotes();

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
    
    Pacote** pacotesSelecionados = new Pacote*[pacotesATransportarCount];
    for(int i = 0; i < pacotesATransportarCount; ++i) {
        pacotesSelecionados[i] = pacotesNaSecao[i];
    }
    
    // --- DEBUG ---
    std::cerr << "[DEBUG]   - Pacotes selecionados para transporte (" << pacotesATransportarCount << "):";
    for(int i = 0; i < pacotesATransportarCount; ++i) {
        std::cerr << " " << pacotesSelecionados[i]->getId();
    }
    std::cerr << std::endl;
    // --- FIM DEBUG ---

    // === INÍCIO DA LÓGICA CORRIGIDA ===

    
    double tempoDaOperacao = relogio + 1.0; 
    PilhaPacotes pilhaTemporaria; // Para pacotes que serão re-empilhados
    
    char msgBuffer[100];
    char idOrigemBuffer[4], idDestinoBuffer[4];
    formatarId(origemId, 3, idOrigemBuffer);
    formatarId(destinoId, 3, idDestinoBuffer);

    // --- FASE 1: Remoção e Particionamento ---
    // Esvazia a pilha da seção, registrando a remoção de cada pacote.
    // Pacotes não selecionados para transporte são guardados em uma pilha temporária.
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

        if(!transportarEste) {
            // Se não for transportado, vai para a pilha temporária para ser re-empilhado.
            pilhaTemporaria.empilhar(pacoteDoTopo);
        }
    }
    
    // --- FASE 2: Log de Transporte e Escalonamento (A CORREÇÃO PRINCIPAL ESTÁ AQUI) ---
    // Itera sobre o array `pacotesSelecionados`, que está na ordem correta (por tempo de postagem).
    for (int i = 0; i < pacotesATransportarCount; ++i) {
        Pacote* pacote = pacotesSelecionados[i]; // Usa o array ordenado
        pacote->avancarRota();
        pacote->setEstado(Pacote::EM_TRANSITO);

        sprintf(msgBuffer, "em transito de %s para %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacote->getId(), msgBuffer);

        Evento* chegadaEv = new ChegadaPacoteEvento(tempoDaOperacao + latenciaTransporte, pacote, destinoId);
        escalonador.Inserir(chegadaEv);
    }

    // --- FASE 3: Rearmazenamento ---
    // Reempilha os pacotes que não foram transportados de volta para a seção.
    while(!pilhaTemporaria.vazia()) {
        Pacote* pacoteParaRearmazenar = pilhaTemporaria.desempilhar();
        pilha->empilhar(pacoteParaRearmazenar);
        sprintf(msgBuffer, "rearmazenado em %s na secao %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacoteParaRearmazenar->getId(), msgBuffer);
    }

    // === FIM DA LÓGICA CORRIGIDA ===

    Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
    escalonador.Inserir(proximoTransporte);
    
    delete[] pacotesNaSecao;
    delete[] pacotesSelecionados;
     // Limpa a memória do array temporário
}

void Simulador::registrarLog(double tempo, int idPacote, const char* mensagem) {
    char logBuffer[256];
    char idPacoteBuffer[4];
    formatarId(idPacote, 3, idPacoteBuffer);
    
    sprintf(logBuffer, "%07d pacote %s %s", static_cast<int>(round(tempo)), idPacoteBuffer, mensagem);
    
    // --- DEBUG ---
    // Imprime o log no stderr no momento da sua criação para vermos a ordem real.
    std::cerr << "[LOG] " << logBuffer << std::endl;
    // --- FIM DEBUG ---

    logSaida->adicionaString(logBuffer);
}

void Simulador::imprimirLogs() {
    logSaida->imprime();
}

void Simulador::formatarId(int id, int largura, char* buffer) {
    sprintf(buffer, "%0*d", largura, id);
}