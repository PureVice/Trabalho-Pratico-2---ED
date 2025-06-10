#include "../include/Fila.h"

// --- Implementação da Classe Fila ---

Fila::Fila() : frente(nullptr), tras(nullptr) {}

Fila::~Fila() {
    while (!vazia()) {
        desenfileirar();
    }
}

// Adiciona um elemento ao final da fila.
void Fila::enfileirar(int valor) {
    NoFila* novo = new NoFila{valor, nullptr};
    if (tras) {
        // Se a fila não está vazia, o último elemento aponta para o novo.
        tras->proximo = novo;
    } else {
        // Se a fila está vazia, o novo elemento é também o primeiro.
        frente = novo;
    }
    // O novo elemento é sempre o último.
    tras = novo;
}

// Remove um elemento do início da fila.
int Fila::desenfileirar() {
    if (frente == nullptr) {
        return -1; // Retorna -1 se a fila estiver vazia.
    }
    NoFila* temp = frente;
    int valor = temp->valor;
    frente = frente->proximo;

    // Se a fila ficou vazia após a remoção, atualiza o ponteiro 'tras'.
    if (frente == nullptr) {
        tras = nullptr;
    }
    
    delete temp;
    return valor;
}

bool Fila::vazia() const {
    return frente == nullptr;
}