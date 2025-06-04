#include "../include/Rede.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

Lista* criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro) {
  Lista* no = (Lista*)malloc(sizeof(Lista));
//no = nó 
  no->valorInteiro = valorInteiro;
  no->valorLista = valorLista;
  no->tipo = tipo;
  no->proximo = NULL;
  return no;
}

void adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro) {
  Lista* novoNo = criaLista(inicio->tipo, valorLista, valorInteiro);
  Lista* aux = inicio;
  while(aux->proximo != NULL){
    aux = aux->proximo;
  }
  aux->proximo = novoNo;
}

void imprimeLista(Lista* inicio) {
  Lista* aux = inicio;
  while (aux != NULL) {
    if(aux->tipo == TIPO_LISTA){
      imprimeLista(aux->valorLista);
    }
    else if(aux->tipo == TIPO_INTEIRO){
      printf("%d ", aux->valorInteiro);
    }
    aux = aux->proximo;
  }
  printf("\n");
}
void deletaLista(Lista* inicio) {
  if(inicio->proximo != NULL){
    deletaLista(inicio->proximo);  
  }
  if(inicio->tipo == TIPO_LISTA && inicio->valorLista != NULL){
    deletaLista(inicio->valorLista);
  }
  free(inicio);
}



Rede::Rede() : nos(nullptr) {
}

Rede::~Rede() {
    if(nos != nullptr) {
        deletaLista(nos);
    }
}

int Rede::QuantidadeArmazens() const {
    int c = 0;
    Lista* aux = nos;
    while (aux != nullptr) {
        c++;
        aux = aux->proximo;
    }
    return c;
}

int Rede::QuantidadeArestas() const {
    int numArestas = 0;
    int count = 0;
    Lista* aux = nos;

    while(aux != nullptr) {
        Lista* no = aux->valorLista;
        while (no != nullptr) {
            numArestas++;
            no = no->proximo;
        }
        aux = aux->proximo;
        count++;
    }

    return numArestas/2;
}

int Rede::GrauMinimo() const {
    int grauMin = 0;
    int count = 0;
    Lista* aux = nos;

    while(aux != nullptr) {
        Lista* no = aux->valorLista;
        int numArestas = 0;

        while (no != nullptr) {
            numArestas++;
            if(count != 0 && numArestas > grauMin) {
                break;
            }
            no = no->proximo;
        }

        if(count == 0 || grauMin > numArestas) {
            count++;
            grauMin = numArestas;
        }
        aux = aux->proximo;
    }
    return grauMin;
}

int Rede::GrauMaximo() const {
    int grauMax = 0;
    int count = 0;
    Lista* aux = nos;

    while(aux != nullptr) {
        Lista* no = aux->valorLista;
        int numArestas = 0;

        while (no != nullptr) {
            numArestas++;
            no = no->proximo;
        }

        if(count == 0 || numArestas > grauMax) {
            count++;
            grauMax = numArestas;
        }
        aux = aux->proximo;
    }

    return grauMax;
}

void Rede::InsereArmazem() {
    int v = QuantidadeArmazens();

    if(nos == nullptr) {
        nos = criaLista(TIPO_LISTA, nullptr, v);
    }
    else {
        adicionaItem(nos, nullptr, v);
    }
}

void Rede::InsereAresta(int v, int w) {
    Lista* aux = nos;
    int c = 0;

    while(c < v) {
        c++;
        aux = aux->proximo;
    }

    if(aux->valorLista == nullptr) {
        aux->valorLista = criaLista(TIPO_INTEIRO, nullptr, w);
    }
    else {
        adicionaItem(aux->valorLista, nullptr, w);
    }
}

void Rede::ImprimeVizinhos(int v) const {
    Lista* aux = nos;
    int c = 0;
    while(c < v) {
        c++;
        aux = aux->proximo;
    }
    imprimeLista(aux->valorLista);
}
