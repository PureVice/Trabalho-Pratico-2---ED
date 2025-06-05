#include "Simulador.h"
#include "Rede.h"
#include "cstdlib"
#include <string>

template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

Escalonador::Escalonador() : raiz(nullptr), ultimo(nullptr), tamanho(0) {}

Escalonador::~Escalonador() {
    // Implementar liberação de memória da heap
}

void Escalonador::subir(HeapEvento* no) {
    while (no->pai && no->evento->getTempo() < no->pai->evento->getTempo()) {
        swap(no->evento, no->pai->evento);
        no = no->pai;
    }
}

// Descer na heap (heapify-down)
void Escalonador::descer(HeapEvento* no) {
    while (true) {
        HeapEvento* menor = no;
        if (no->esquerda && no->esquerda->evento->getTempo() < menor->evento->getTempo()) {
            menor = no->esquerda;
        }
        if (no->direita && no->direita->evento->getTempo() < menor->evento->getTempo()) {
            menor = no->direita;
        }
        if (menor == no) break;
        swap(no->evento, menor->evento);
        no = menor;
    }
}

// Encontrar o nó pai onde o próximo nó será inserido (BFS reverso simulada)
HeapEvento* Escalonador::encontrarPaiInsercao() {
    // Converte o tamanho+1 em caminho binário e percorre para achar o pai
    int pos = tamanho + 1;
    std::string caminho;
    while (pos > 1) {
        caminho = (pos % 2 == 0 ? 'L' : 'R') + caminho;
        pos /= 2;
    }
    HeapEvento* atual = raiz;
    for (int i = 0; i < (int)caminho.length() - 1; i++) {
        if (caminho[i] == 'L') atual = atual->esquerda;
        else atual = atual->direita;
    }
    return atual;
}

// Encontrar o novo último nó após remoção (caminho binário reverso)
HeapEvento* Escalonador::encontrarUltimo() {
    int pos = tamanho;
    std::string caminho;
    while (pos > 1) {
        caminho = (pos % 2 == 0 ? 'L' : 'R') + caminho;
        pos /= 2;
    }
    HeapEvento* atual = raiz;
    for (char c : caminho) {
        if (c == 'L') atual = atual->esquerda;
        else atual = atual->direita;
    }
    return atual;
}

void Escalonador::insereEvento(Evento* evento) {
    HeapEvento* novo = new HeapEvento{evento, nullptr, nullptr, nullptr};
    
    if (raiz == nullptr) {
        raiz = ultimo = novo;
        tamanho++;
        return;
    }
    
    HeapEvento* pai = encontrarPaiInsercao();
    novo->pai = pai;
    
    if (pai->esquerda == nullptr) {
        pai->esquerda = novo;
    } else {
        pai->direita = novo;
    }
    
    ultimo = novo;
    tamanho++;
    subir(novo);
}

Evento* Escalonador::retiraProximoEvento() {
    if (raiz == nullptr) return nullptr;
    
    Evento* proximo = raiz->evento;
    if (tamanho == 1) {
        delete raiz;
        raiz = ultimo = nullptr;
        tamanho = 0;
        return proximo;
    }
    
    // Troca raiz com último
    raiz->evento = ultimo->evento;
    
    // Remove último
    HeapEvento* paiUltimo = ultimo->pai;
    if (paiUltimo->direita == ultimo) {
        paiUltimo->direita = nullptr;
    } else {
        paiUltimo->esquerda = nullptr;
    }
    
    delete ultimo;
    tamanho--;
    
    // Atualiza último
    ultimo = encontrarUltimo();
    
    // Ajusta heap
    descer(raiz);
    
    return proximo;
}

bool Escalonador::vazio() const {
    return tamanho == 0;
}

// Implementar métodos auxiliares...

#include "Simulador.h"
#include <iostream>
#include <fstream>

Simulador::Simulador(Rede& rede, int numArmazens) 
    : rede(rede), numArmazens(numArmazens), relogio(0) {
    
    // Inicializar armazéns
    armazens = new Armazem*[numArmazens];
    for (int i = 0; i < numArmazens; i++) {
        armazens[i] = new Armazem(i, numArmazens);
    }
}

Simulador::~Simulador() {
    for (int i = 0; i < numArmazens; i++) {
        delete armazens[i];
    }
    delete[] armazens;
}

void Simulador::carregarDados(const char* arquivo) {
    
}

void Simulador::inicializarTransportes() {
    // Para cada ligação entre armazéns (aresta)
    Lista* no = rede.getNos();
    for (int i = 0; i < numArmazens; i++) {
        Lista* vizinhos = no->valorLista;
        while (vizinhos != nullptr) {
            int destino = vizinhos->valorInteiro;
            if (i < destino) { // Evitar duplicatas
                // Capacidade e frequência fixas para simplificação
                TransporteEvento* evento = new TransporteEvento(relogio, i, destino, 10);
                escalonador.insereEvento(evento);
            }
            vizinhos = vizinhos->proximo;
        }
        no = no->proximo;
    }
}

void Simulador::executar() {
    inicializarTransportes();
    
    while (!escalonador.vazio()) {
        Evento* evento = escalonador.retiraProximoEvento();
        relogio = evento->getTempo();
        
        switch (evento->getTipo()) {
            case CHEGADA_PACOTE:
                processarChegadaPacote(static_cast<ChegadaPacoteEvento*>(evento));
                break;
            case TRANSPORTE:
                processarTransporte(static_cast<TransporteEvento*>(evento));
                break;
        }
        
        delete evento;
    }
}

void Simulador::processarChegadaPacote(ChegadaPacoteEvento* evento) {
    Pacote* pacote = evento->getPacote();
    int armazem = evento->getArmazem();
    
    if (armazem == pacote->getDestino()) {
        pacote->setEstado(Pacote::ENTREGUE);
        pacote->registrarTempoEntrega(relogio);
    } else {
        int proximo = pacote->getProximoArmazemRota();
        armazens[armazem]->armazenarPacote(proximo, pacote, relogio);
        pacote->setEstado(Pacote::ARMAZENADO);
        pacote->avancarRota();
    }
}

void Simulador::processarTransporte(TransporteEvento* evento) {
    int origem = evento->getOrigem();
    int destino = evento->getDestino();
    int capacidade = evento->getCapacidade();
    
    // Recuperar pacotes da seção correspondente
    PilhaPacotes* secao = armazens[origem]->getSecao(destino);
    if (!secao || secao->vazia()) {
        // Reagendar transporte se não houver pacotes
        TransporteEvento* novoEvento = new TransporteEvento(
            relogio + 1.0, // Frequência de 1 hora
            origem,
            destino,
            capacidade
        );
        escalonador.insereEvento(novoEvento);
        return;
    }
    
    // Remover pacotes até a capacidade do transporte
    int cont = 0;
    while (cont < capacidade && !secao->vazia()) {
        Pacote* pacote = secao->desempilhar();
        pacote->setEstado(Pacote::ALOCADO_TRANSPORTE);
        
        // Calcular tempo de viagem (fixo em 2 horas para simplificação)
        double tempoChegada = relogio + 2.0;
        
        // Escalonar chegada no próximo armazém
        ChegadaPacoteEvento* novoEvento = new ChegadaPacoteEvento(
            tempoChegada,
            pacote,
            destino
        );
        escalonador.insereEvento(novoEvento);
        cont++;
    }
    
    // Reagendar próximo transporte
    TransporteEvento* novoEvento = new TransporteEvento(
        relogio + 1.0, // Frequência de 1 hora
        origem,
        destino,
        capacidade
    );
    escalonador.insereEvento(novoEvento);
}

void Simulador::calcularERegistrarRota(Pacote* pacote) {
    int origem = pacote->getOrigem();
    int destino = pacote->getDestino();
    int tamanhoRota;
    
    // Calcular rota usando BFS (já implementado anteriormente)

    int* rota = rede.calculaRota(rede, origem, destino, numArmazens, tamanhoRota);
    
    // Armazenar rota no pacote
    pacote->setRota(rota, tamanhoRota);
}