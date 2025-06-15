#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <cstring>

//inicializa os membros e dispara o carregamento de dados.
Simulador::Simulador(const char* arquivoEntrada) {
    this->rede = nullptr;
    this->armazens = nullptr;
    this->relogio = 0.0;
    this->pacotesAtivos = 0;
    this->logSaida = new Lista();
    this->todosOsPacotes = nullptr;
    this->numTotalPacotes = 0;
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

    // Garante que todos os pacotes sejam liberados, evitando vazamento de memória.
    if (todosOsPacotes != nullptr) {
        for (int i = 0; i < numTotalPacotes; ++i) {
            delete todosOsPacotes[i];
        }
        delete[] todosOsPacotes;
    }
}

// carregarDados: lê o arquivo de entrada, popula as estruturas de dados e agenda os eventos iniciais.
void Simulador::carregarDados(const char* arquivoEntrada) {
    std::ifstream arquivo(arquivoEntrada);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir o arquivo de entrada: " << arquivoEntrada << std::endl;
        exit(EXIT_FAILURE);
    }

    // Função auxiliar (lambda) para verificar falhas de leitura e encerrar com erro.
    auto verificarFalhaLeitura = [&](const char* parametro) {
        if (!arquivo) {
            std::cerr << "ERRO: Falha ao ler o parametro '" << parametro << "'. Arquivo mal formatado." << std::endl;
            exit(EXIT_FAILURE);
        }
    };

    // Leitura dos parâmetros globais da simulação.
    arquivo >> capacidadeTransporte;
    verificarFalhaLeitura("capacidadeTransporte");
    arquivo >> latenciaTransporte;
    verificarFalhaLeitura("latenciaTransporte");
    arquivo >> intervaloTransportes;
    verificarFalhaLeitura("intervaloTransportes");
    arquivo >> custoRemocao;
    verificarFalhaLeitura("custoRemocao");

    // Leitura e construção da topologia da rede de armazéns.
    arquivo >> numArmazens;
    verificarFalhaLeitura("numArmazens");
    this->rede = new Rede(numArmazens);
    this->armazens = new Armazem*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        armazens[i] = new Armazem(i, numArmazens);
        for (int j = 0; j < numArmazens; ++j) {
            int conectado;
            arquivo >> conectado;
             if (!arquivo) {
                std::cerr << "ERRO: Falha ao ler a matriz de adjacencia na posicao [" << i << "][" << j << "]." << std::endl;
                exit(EXIT_FAILURE);
            }
            if (conectado == 1) {
                rede->InsereAresta(i, j);
            }
        }
    }
    
    // Leitura dos pacotes, cálculo de suas rotas e agendamento da chegada inicial.
    int numPacotes;
    arquivo >> numPacotes;
    verificarFalhaLeitura("numPacotes");
    this->numTotalPacotes = numPacotes;
    this->pacotesAtivos = this->numTotalPacotes;

    this->todosOsPacotes = new Pacote*[this->numTotalPacotes];

    for (int i = 0; i < this->numTotalPacotes; ++i) {
        double tempoPostagem;
        int idPacote, idOrigem, idDestino;
        char buffer[10]; // Buffer para consumir os delimitadores "p", "a", "d".
        arquivo >> tempoPostagem >> buffer >> idPacote >> buffer >> idOrigem >> buffer >> idDestino;
        
        if (!arquivo) {
             std::cerr << "ERRO: Falha ao ler os dados do pacote de indice " << i << ". Arquivo mal formatado." << std::endl;
             exit(EXIT_FAILURE);
        }

        idPacote = i; // Sobrescreve o ID do arquivo para garantir unicidade.
        Pacote* p = new Pacote(idPacote, idOrigem, idDestino, tempoPostagem);
        p->setRota(calculaRota(*rede, idOrigem, idDestino)); // Rota mais curta via BFS.
        p->setEstado(Pacote::CHEGADA_AGENDADA);
        
        // Armazena ponteiro para garantir a liberação da memória no final.
        this->todosOsPacotes[i] = p;

        // Insere o primeiro evento (chegada na origem) na fila de prioridades.
        Evento* ev = new ChegadaPacoteEvento(tempoPostagem, p, idOrigem);
        escalonador.Inserir(ev);
    }
    arquivo.close();
}

// executar: contém o laço principal da simulação.
void Simulador::executar() {
    bool primeiroPacoteProcessado = false;
    // O laço continua enquanto houver eventos pendentes e pacotes não entregues.
    while (!escalonador.Vazio() && pacotesAtivos > 0)
    {
        // Remove o próximo evento cronológico da fila de prioridades.
        Evento *proximoEvento = escalonador.Remover();
        
        // Lógica para inicializar os transportes a partir do tempo do primeiro evento de pacote.
        if (!primeiroPacoteProcessado && proximoEvento->getTipo() == CHEGADA_PACOTE)
        {
            tempoPrimeiroPacote = proximoEvento->getTempo();
            inicializarTransportes(tempoPrimeiroPacote);
            primeiroPacoteProcessado = true;
        }

        // Avança o relógio da simulação para o tempo do evento atual.
        if (proximoEvento->getTempo() > relogio)
        {
            relogio = proximoEvento->getTempo();
        }

        // Delega o processamento do evento para a função apropriada.
        if (proximoEvento->getTipo() == CHEGADA_PACOTE)
        {
            processarChegadaPacote(static_cast<ChegadaPacoteEvento *>(proximoEvento));
        }
        else if (proximoEvento->getTipo() == TRANSPORTE)
        {
            processarTransporte(static_cast<TransporteEvento *>(proximoEvento));
        }
        delete proximoEvento; // Libera a memória do objeto evento processado.
    }
    imprimirLogs(); // Imprime todos os logs acumulados ao final.
}

// inicializarTransportes: agenda o primeiro evento de transporte para cada rota da rede.
void Simulador::inicializarTransportes(double tempoInicial) {
    for (int i = 0; i < numArmazens; ++i) {
        Lista* vizinhos = rede->getVizinhos(i);
        Celula* atual = vizinhos->getInicio();
        while(atual != nullptr) {
            int vizinhoId = atual->valorInteiro;
            // A condição i < vizinhoId evita agendar rotas duplicadas (ex: 0->1 e 1->0).
            // Em vez disso, agendamos ambas as direções de uma vez.
            if (i < vizinhoId) {
                Evento* ev = new TransporteEvento(tempoInicial + intervaloTransportes, i, vizinhoId);
                escalonador.Inserir(ev);
                Evento* ev_rev = new TransporteEvento(tempoInicial + intervaloTransportes, vizinhoId, i);
                escalonador.Inserir(ev_rev);
            }
            atual = atual->proximo;
        }
    }
}

// processarChegadaPacote: trata a chegada de um pacote a um armazém.
void Simulador::processarChegadaPacote(ChegadaPacoteEvento* evento) {
    Pacote* pacote = evento->getPacote();
    int armazemId = evento->getArmazemChegada();
    
    char msgBuffer[100];
    char idBuffer[4];

    // Verifica se o armazém atual é o destino final do pacote.
    if (armazemId == pacote->getDestino()) {
        pacote->setEstado(Pacote::ENTREGUE);
        formatarId(armazemId, 3, idBuffer);
        sprintf(msgBuffer, "entregue em %s", idBuffer);
        registrarLog(evento->getTempo(), pacote->getId(), msgBuffer);
        pacotesAtivos--; // Decrementa o contador de pacotes ativos.
    
    } else {
        // Se não for o destino, armazena o pacote na seção correta.
        armazens[armazemId]->armazenarPacote(pacote);
        char idBufferProx[4];
        formatarId(armazemId, 3, idBuffer);
        formatarId(pacote->getProximoSalto(), 3, idBufferProx);
        sprintf(msgBuffer, "armazenado em %s na secao %s", idBuffer, idBufferProx);
        registrarLog(evento->getTempo(), pacote->getId(), msgBuffer);
    }
}

// processarTransporte: simula a operação de um veículo entre dois armazéns.
void Simulador::processarTransporte(TransporteEvento* evento) {
    int origemId = evento->getOrigem();
    int destinoId = evento->getDestino();

    // Reagenda o próximo evento de transporte para esta rota, mantendo o ciclo.
    Evento* proximoTransporte = new TransporteEvento(relogio + intervaloTransportes, origemId, destinoId);
    escalonador.Inserir(proximoTransporte);

    Secao* secao = armazens[origemId]->getSecaoPorDestino(destinoId);
    if (secao == nullptr || secao->estaVazia()) {
        return; // Nenhum pacote para transportar.
    }

    PilhaPacotes* pilha = secao->getPilha();
    int numPacotesNaSecao = pilha->getTamanho();
    Pacote** pacotesNaSecaoLIFO = pilha->getPacotes();

    // Determina quantos pacotes transportar, limitado pela capacidade do veículo.
    int pacotesATransportarCount = (numPacotesNaSecao < capacidadeTransporte) ? numPacotesNaSecao : capacidadeTransporte;
    
    // Simula uma seleção FIFO (primeiro a entrar, primeiro a sair) a partir da pilha LIFO.
    // Os pacotes que chegaram primeiro estão no fundo da pilha (final do array).
    Pacote** pacotesSelecionados = new Pacote*[pacotesATransportarCount];
    for (int i = 0; i < pacotesATransportarCount; ++i) {
        pacotesSelecionados[i] = pacotesNaSecaoLIFO[numPacotesNaSecao - 1 - i];
    }
    
    double tempoDaOperacao = relogio;
    PilhaPacotes pilhaTemporaria;
    
    char msgBuffer[100];
    char idOrigemBuffer[4], idDestinoBuffer[4];
    formatarId(origemId, 3, idOrigemBuffer);
    formatarId(destinoId, 3, idDestinoBuffer);

    // FASE 1: Desempilha todos os pacotes, separando os que serão transportados.
    while (!pilha->vazia()) {
        Pacote* pacoteDoTopo = pilha->desempilhar();
        tempoDaOperacao += custoRemocao; // Adiciona custo de remoção.
        
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
            pilhaTemporaria.empilhar(pacoteDoTopo); // Guarda para reempilhar depois.
        }
    }
    
    // FASE 2: Processa os pacotes selecionados, agendando sua chegada ao destino.
    for (int i = 0; i < pacotesATransportarCount; ++i) {
        Pacote* pacote = pacotesSelecionados[i];
        pacote->avancarRota();
        pacote->setEstado(Pacote::EM_TRANSITO);

        sprintf(msgBuffer, "em transito de %s para %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacote->getId(), msgBuffer);

        Evento* chegadaEv = new ChegadaPacoteEvento(tempoDaOperacao + latenciaTransporte, pacote, destinoId);
        escalonador.Inserir(chegadaEv);
    }

    // FASE 3: Reempilha os pacotes que não foram transportados de volta na seção.
    while (!pilhaTemporaria.vazia()) {
        Pacote* pacoteParaRearmazenar = pilhaTemporaria.desempilhar();
        pilha->empilhar(pacoteParaRearmazenar);
        sprintf(msgBuffer, "rearmazenado em %s na secao %s", idOrigemBuffer, idDestinoBuffer);
        registrarLog(tempoDaOperacao, pacoteParaRearmazenar->getId(), msgBuffer);
    }
    
    // Libera a memória dos arrays temporários.
    delete[] pacotesNaSecaoLIFO;
    delete[] pacotesSelecionados;
}

// registrarLog: formata e armazena uma string de log para impressão final.
void Simulador::registrarLog(double tempo, int idPacote, const char* mensagem) {
    char logBuffer[256];
    char idPacoteBuffer[4];
    formatarId(idPacote, 3, idPacoteBuffer);
    
    // Formata o tempo para um inteiro arredondado com 7 dígitos.
    sprintf(logBuffer, "%07d pacote %s %s", static_cast<int>(round(tempo)), idPacoteBuffer, mensagem);
    
    logSaida->adicionaString(logBuffer);
}

// imprimirLogs: imprime todos os logs armazenados na lista.
void Simulador::imprimirLogs() {
    logSaida->imprime();
}

// formatarId: formata um número inteiro para uma string com preenchimento de zeros à esquerda.
void Simulador::formatarId(int id, int largura, char* buffer) {
    sprintf(buffer, "%0*d", largura, id);
}