#ifndef LISTA_H
#define LISTA_H
#include <iostream>



enum TipoVariavel {
    TIPO_LISTA,
    TIPO_INTEIRO
};

// Estrutura genérica de nó para listas encadeadas
// Usada tanto para a lista de armazéns quanto para as listas de adjacência
struct Lista {
    int valorInteiro;
    Lista* valorLista;
    TipoVariavel tipo;
    Lista* proximo;
};

Lista* criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro) {
    Lista* no = new Lista;
    no->valorInteiro = valorInteiro;
    no->valorLista = valorLista;
    no->tipo = tipo;
    no->proximo = nullptr;
    return no;
}

void adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro) {
    Lista* novoNo = criaLista(inicio->tipo, valorLista, valorInteiro);
    Lista* aux = inicio;
    while (aux->proximo != nullptr) {
        aux = aux->proximo;
    }
    aux->proximo = novoNo;
}

void imprimeLista(Lista* inicio) {
    Lista* aux = inicio;
    while (aux != nullptr) {
        if (aux->tipo == TIPO_INTEIRO) {
            std::cout << aux->valorInteiro << " ";
        }
        aux = aux->proximo;
    }
    std::cout << std::endl;
}

void deletaLista(Lista* inicio) {
    Lista* atual = inicio;
    while (atual != nullptr) {
        Lista* proximo = atual->proximo;
        delete atual;
        atual = proximo;
    }
}


#endif