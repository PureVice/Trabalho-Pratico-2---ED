#include "../include/Rede.h"
#include "../include/Fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>


int* calculaRota(Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota) {
    // Arrays para controle do BFS
    bool* visitados = new bool[numArmazens];
    int* predecessores = new int[numArmazens];
    
    for (int i = 0; i < numArmazens; i++) {
        visitados[i] = false;
        predecessores[i] = -1;
    }
    
    Fila fila;
    fila.enfileirar(origem);
    visitados[origem] = true;
    
    while (!fila.vazia()) {
        int atual = fila.desenfileirar();
        
        if (atual == destino) break;
        
        // Obtém vizinhos do armazém atual
        Lista* vizinhos = getVizinhos(rede, atual);
        while (vizinhos != nullptr) {
            int vizinho = vizinhos->valorInteiro;
            if (!visitados[vizinho]) {
                visitados[vizinho] = true;
                predecessores[vizinho] = atual;
                fila.enfileirar(vizinho);
            }
            vizinhos = vizinhos->proximo;
        }
    }
    
    // Reconstrói a rota
    int* rota = nullptr;
    tamanhoRota = 0;
    
    if (predecessores[destino] != -1) {
        // Conta o tamanho da rota
        int cont = 0;
        int atual = destino;
        while (atual != -1) {
            cont++;
            atual = predecessores[atual];
        }
        
        tamanhoRota = cont;
        rota = new int[tamanhoRota];
        
        // Preenche a rota na ordem correta
        atual = destino;
        for (int i = tamanhoRota - 1; i >= 0; i--) {
            rota[i] = atual;
            atual = predecessores[atual];
        }
    }
    
    delete[] visitados;
    delete[] predecessores;
    
    return rota;
}


Lista* getVizinhos(Rede& rede, int armazem) {
    Lista* aux = rede.getNos();
    int c = 0;
    while (c < armazem && aux != nullptr) {
        c++;
        aux = aux->proximo;
    }
    if (aux == nullptr) return nullptr;
    return aux->valorLista;
}
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
    // Função auxiliar para inserção em um vértice específico
    auto inserirEmVertice = [this](int vertice, int valor) {
        Lista* aux = this->nos;
        int c = 0;
        while(c < vertice) {
            c++;
            aux = aux->proximo;
        }

        if(aux->valorLista == nullptr) {
            aux->valorLista = criaLista(TIPO_INTEIRO, nullptr, valor);
        }
        else {
            // Verificar se a aresta já existe para não inserir duplicatas
            Lista* temp = aux->valorLista;
            while(temp != nullptr) {
                if(temp->valorInteiro == valor) break;
                temp = temp->proximo;
            }
            if(temp == nullptr) { // Ainda não existe
                adicionaItem(aux->valorLista, nullptr, valor);
            }
        }
    };

    // Inserir w na lista de v
    inserirEmVertice(v, w);
    
    // Inserir v na lista de w (recíproco)
    inserirEmVertice(w, v);
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
