#ifndef LISTA_H
#define LISTA_H

enum TipoVariavel
{
    TIPO_LISTA,
    TIPO_INTEIRO
};

struct Lista
{
    int valorInteiro;
    Lista *valorLista;
    TipoVariavel tipo;
    Lista *proximo;
};

Lista *criaLista(TipoVariavel tipo, Lista *valorLista, int valorInteiro);
void adicionaItem(Lista *inicio, Lista *valorLista, int valorInteiro);
void imprimeLista(Lista *inicio);
void deletaLista(Lista *inicio);
void imprimeRotas(Lista *rota);
#endif // LISTA_H