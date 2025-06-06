#include "Fila.h"

Fila::Fila() : frente(nullptr), tras(nullptr) {}

Fila::~Fila() {
    while (!vazia()) {
        desenfileirar();
    }
}

void Fila::enfileirar(int valor) {
    NoFila* novo = new NoFila{valor, nullptr};
    if (tras) {
        tras->proximo = novo;
    } else {
        frente = novo;
    }
    tras = novo;
}

int Fila::desenfileirar() {
    if (frente == nullptr) return -1;
    NoFila* temp = frente;
    int valor = temp->valor;
    frente = frente->proximo;
    if (frente == nullptr) tras = nullptr;
    delete temp;
    return valor;
}

bool Fila::vazia() const {
    return frente == nullptr;
}