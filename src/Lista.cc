#include "Lista.h"
#include <iostream>

Lista *criaLista(TipoVariavel tipo, Lista *valorLista, int valorInteiro)
{
    Lista *no = new Lista;
    no->valorInteiro = valorInteiro;
    no->valorLista = valorLista;
    no->tipo = tipo;
    no->proximo = nullptr;
    return no;
}

void adicionaItem(Lista *inicio, Lista *valorLista, int valorInteiro)
{
    Lista *novoNo = criaLista(inicio->tipo, valorLista, valorInteiro);
    Lista *aux = inicio;
    while (aux->proximo != nullptr)
    {
        aux = aux->proximo;
    }
    aux->proximo = novoNo;
}

void imprimeLista(Lista *inicio)
{
    Lista *aux = inicio;
    while (aux != nullptr)
    {
        if (aux->tipo == TIPO_INTEIRO)
        {
            std::cout << aux->valorInteiro << " ";
        }
        aux = aux->proximo;
    }
    std::cout << std::endl;
}

void deletaLista(Lista *inicio)
{
    Lista *atual = inicio;
    while (atual != nullptr)
    {
        Lista *proximo = atual->proximo;
        delete atual;
        atual = proximo;
    }
}
void imprimeRotas(Lista *rota)
{
    if (rota == nullptr)
    {
        std::cout << "Rota não existe!" << std::endl;
        return;
    }

    Lista *atual = rota;
    while (atual != nullptr)
    {
        std::cout << atual->valorInteiro;
        if (atual->proximo != nullptr)
        {
            std::cout << " -> ";
        }
        atual = atual->proximo;
    }
    std::cout << std::endl;
}