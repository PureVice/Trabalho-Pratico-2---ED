#include "../include/Escalonador.h"
#include <iostream>

// As funções de obter posições permanecem as mesmas
int Escalonador::GetAncestral(int posicao) {
    if (posicao > 0) {
        return (posicao - 1) / 2;
    }
    return 0;
}

int Escalonador::GetSucessorEsq(int posicao) {
    return 2 * posicao + 1;
}

int Escalonador::GetSucessorDir(int posicao) {
    return 2 * posicao + 2;
}

// A lógica de Heapify é a mesma, mas a COMPARAÇÃO e a TROCA mudam
void Escalonador::HeapifyPorBaixo(int posicao) {
    int esq = GetSucessorEsq(posicao);
    int dir = GetSucessorDir(posicao);

    if (esq >= tamanho) return;

    int menor = esq;

    // Compara o TEMPO dos eventos
    if (dir < tamanho && data[dir]->getTempo() < data[esq]->getTempo()) {
        menor = dir;
    }

    // Compara o TEMPO dos eventos
    if (data[menor]->getTempo() < data[posicao]->getTempo()) {
        // Troca os ponteiros
        Evento* temp = data[posicao];
        data[posicao] = data[menor];
        data[menor] = temp;

        HeapifyPorBaixo(menor);
    }
}

void Escalonador::HeapifyPorCima(int i) {
    if (i > 0 && data[i]->getTempo() < data[GetAncestral(i)]->getTempo()) {
        // Troca os ponteiros
        Evento* temp = data[i];
        data[i] = data[GetAncestral(i)];
        data[GetAncestral(i)] = temp;

        HeapifyPorCima(GetAncestral(i));
    }
}

Escalonador::Escalonador(int capacidadeInicial) {
    this->capacidade = capacidadeInicial;
    this->data = new Evento*[capacidade];
    this->tamanho = 0;
}

Escalonador::~Escalonador() {
    // É crucial deletar cada objeto Evento antes de deletar o array
    for (int i = 0; i < tamanho; ++i) {
        delete data[i];
    }
    delete[] data;
}

void Escalonador::redimensionar() {
    int novaCapacidade = capacidade * 2;
    Evento** newData = new Evento*[novaCapacidade];
    for (int i = 0; i < tamanho; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacidade = novaCapacidade;
}

void Escalonador::Inserir(Evento* ev) {
    if (tamanho == capacidade) {
        redimensionar();
    }
    data[tamanho] = ev;
    HeapifyPorCima(tamanho);
    tamanho++;
}

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