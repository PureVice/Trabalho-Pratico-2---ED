#include "../include/Utils.h"
#include <iostream>
#include <cstring> // Adicionado para strlen e strcpy

// --- Implementação da Classe Lista ---

Lista::Lista() : inicio(nullptr), fim(nullptr) {}

Lista::~Lista() {
    Celula* atual = inicio;
    while (atual != nullptr) {
        Celula* proximo = atual->proximo;
        // Libera a memória alocada para a string se for do tipo TIPO_STRING
        if (atual->tipo == TIPO_STRING && atual->valorString != nullptr) {
            delete[] atual->valorString;
        }
        delete atual;
        atual = proximo;
    }
}

void Lista::adicionaInteiro(int valor) {
    Celula* nova = new Celula();
    nova->tipo = TIPO_INTEIRO;
    nova->valorInteiro = valor;
    nova->proximo = nullptr;

    if (vazia()) {
        inicio = fim = nova;
    } else {
        fim->proximo = nova;
        fim = nova;
    }
}

void Lista::adicionaString(const char* valor) {
    Celula* nova = new Celula();
    nova->tipo = TIPO_STRING;
    // Aloca memória para a nova string e copia o conteúdo
    if (valor) {
        nova->valorString = new char[strlen(valor) + 1];
        strcpy(nova->valorString, valor);
    } else {
        nova->valorString = nullptr;
    }
    nova->proximo = nullptr;

    if (vazia()) {
        inicio = fim = nova;
    } else {
        fim->proximo = nova;
        fim = nova;
    }
}

bool Lista::vazia() const {
    return inicio == nullptr;
}

Celula* Lista::getInicio() const {
    return inicio;
}

// Método genérico para imprimir a lista, tratando os dois tipos.
// Útil para depuração.
void Lista::imprime() const {
    Celula* aux = inicio;
    while (aux != nullptr) {
        if (aux->tipo == TIPO_INTEIRO) {
            std::cout << aux->valorInteiro << " ";
        } else if (aux->tipo == TIPO_STRING) {
            // Para strings (logs), imprime cada uma em uma nova linha.
            if(aux->valorString) {
                std::cout << aux->valorString << std::endl;
            }
        }
        aux = aux->proximo;
    }
    // Adiciona uma quebra de linha no final se for uma lista de inteiros.
    if(inicio && inicio->tipo == TIPO_INTEIRO) {
        std::cout << std::endl;
    }
}

// Método específico para imprimir uma lista que representa uma rota.
void Lista::imprimeRota() const {
    if (vazia()) {
        std::cout << "Rota não existe!" << std::endl;
        return;
    }

    Celula* atual = inicio;
    while (atual != nullptr) {
        // Assume que a rota só contém inteiros.
        if (atual->tipo == TIPO_INTEIRO) {
            std::cout << atual->valorInteiro;
            if (atual->proximo != nullptr) {
                std::cout << " -> ";
            }
        }
        atual = atual->proximo;
    }
    std::cout << std::endl;
}


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
