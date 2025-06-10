#include "../include/Escalonador.h"
#include <iostream>

// --- Implementação da Classe Escalonador (Min-Heap) ---

Escalonador::Escalonador(int capacidadeInicial) {
    this->capacidade = capacidadeInicial;
    this->data = new Evento*[capacidade];
    this->tamanho = 0;
}

Escalonador::~Escalonador() {
    // É crucial deletar cada objeto Evento que ainda resta na heap.
    for (int i = 0; i < tamanho; ++i) {
        delete data[i];
    }
    delete[] data;
}

// Insere um novo evento na heap, mantendo a propriedade de ordenação.
void Escalonador::Inserir(Evento* ev) {
    if (tamanho == capacidade) {
        redimensionar();
    }
    data[tamanho] = ev;
    HeapifyPorCima(tamanho);
    tamanho++;
}

// Remove e retorna o evento com a menor chave de prioridade.
Evento* Escalonador::Remover() {
    if (Vazio()) {
        return nullptr;
    }

    Evento* raiz = data[0];
    
    data[0] = data[tamanho - 1];
    tamanho--;
    if (!Vazio()) {
        HeapifyPorBaixo(0);
    }
    return raiz;
}

bool Escalonador::Vazio() const {
    return tamanho == 0;
}


// --- Métodos Privados da Heap ---

int Escalonador::GetAncestral(int posicao) {
    return (posicao - 1) / 2;
}

int Escalonador::GetSucessorEsq(int posicao) {
    return 2 * posicao + 1;
}

int Escalonador::GetSucessorDir(int posicao) {
    return 2 * posicao + 2;
}

// Reorganiza a heap de cima para baixo.
void Escalonador::HeapifyPorBaixo(int posicao) {
    int esq = GetSucessorEsq(posicao);
    int dir = GetSucessorDir(posicao);

    if (esq >= tamanho) return; // Nó é uma folha.

    int menor = esq;

    if (dir < tamanho && data[dir]->getPrioridade() < data[esq]->getPrioridade()) {
        menor = dir;
    }

    if (data[menor]->getPrioridade() < data[posicao]->getPrioridade()) {
        Evento* temp = data[posicao];
        data[posicao] = data[menor];
        data[menor] = temp;

        HeapifyPorBaixo(menor);
    }
}

// Reorganiza a heap de baixo para cima.
void Escalonador::HeapifyPorCima(int posicao) {
    if (posicao == 0) return;

    int ancestral = GetAncestral(posicao);

    if (data[posicao]->getPrioridade() < data[ancestral]->getPrioridade()) {
        Evento* temp = data[posicao];
        data[posicao] = data[ancestral];
        data[ancestral] = temp;

        HeapifyPorCima(ancestral);
    }
}

// Dobra a capacidade do array da heap quando ele fica cheio.
void Escalonador::redimensionar() {
    int novaCapacidade = (capacidade == 0) ? 1 : capacidade * 2;
    Evento** newData = new Evento*[novaCapacidade];
    for (int i = 0; i < tamanho; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacidade = novaCapacidade;
}