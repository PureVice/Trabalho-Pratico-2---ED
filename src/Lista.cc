#include "../include/Lista.h"
#include <iostream>

// --- Implementação da Classe Lista ---

Lista::Lista() : inicio(nullptr), fim(nullptr) {}

Lista::~Lista() {
    Celula* atual = inicio;
    while (atual != nullptr) {
        Celula* proximo = atual->proximo;
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

void Lista::adicionaString(const std::string& valor) {
    Celula* nova = new Celula();
    nova->tipo = TIPO_STRING;
    nova->valorString = valor;
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
            std::cout << aux->valorString << std::endl;
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