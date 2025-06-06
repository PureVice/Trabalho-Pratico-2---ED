#include "Rede.h"
#include "Fila.h" // Assumindo que Fila.h exista e funcione
#include <iostream>
#include <cstdlib>

<<<<<<< HEAD
Lista* Rede::criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro) {
  Lista* no = (Lista*)malloc(sizeof(Lista));
//no = nó 
  no->valorInteiro = valorInteiro;
  no->valorLista = valorLista;
  no->tipo = tipo;
  no->proximo = NULL;
  return no;
}

void Rede::adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro) {
  Lista* novoNo = criaLista(inicio->tipo, valorLista, valorInteiro);
  Lista* aux = inicio;
  while(aux->proximo != NULL){
    aux = aux->proximo;
  }
  aux->proximo = novoNo;
}

void Rede::imprimeLista(Lista* inicio) {
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
void Rede::deletaLista(Lista* inicio) {
  if(inicio->proximo != NULL){
    deletaLista(inicio->proximo);  
  }
  if(inicio->tipo == TIPO_LISTA && inicio->valorLista != NULL){
    deletaLista(inicio->valorLista);
  }
  free(inicio);
}



Rede::Rede() : nos(nullptr) {
=======
// ----------------- Implementação da Classe Rede -----------------

// Construtor: Aloca o array de listas de adjacência
Rede::Rede(int num_armazens_total) {
    this->capacidade = num_armazens_total;
    this->numArmazens = num_armazens_total; // Assume que todos serão usados
    this->adjacencias = new Lista*[capacidade];
    for (int i = 0; i < capacidade; ++i) {
        adjacencias[i] = nullptr; // Inicializa todas as listas como nulas
    }
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
}

// Destrutor: Libera a memória de cada lista e do array principal
Rede::~Rede() {
    for (int i = 0; i < numArmazens; ++i) {
        if (adjacencias[i] != nullptr) {
            deletaLista(adjacencias[i]);
        }
    }
    delete[] adjacencias;
}

<<<<<<< HEAD
int Rede::QuantidadeArmazens(Rede* rede) {
    int c = 0;
    Lista* aux = rede->nos;
    while (aux != nullptr) {
        c++;
        aux = aux->proximo;
=======
// Retorna o número de armazéns
int Rede::QuantidadeArmazens() const {
    return this->numArmazens;
}

// Insere uma aresta de forma eficiente
void Rede::InsereAresta(int v, int w) {
    // Validação básica de limites
    if (v < 0 || v >= numArmazens || w < 0 || w >= numArmazens) return;

    // Acesso direto em O(1) à lista de adjacência do armazém 'v'
    if (adjacencias[v] == nullptr) {
        adjacencias[v] = criaLista(TIPO_INTEIRO, nullptr, w);
    } else {
        adicionaItem(adjacencias[v], nullptr, w);
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
    }
}

<<<<<<< HEAD
int Rede::QuantidadeArestas(Rede* rede) {
    int numArestas = 0;
    int count = 0;
    Lista* aux = rede->nos;

    while(aux != nullptr) {
        Lista* no = aux->valorLista;
        while (no != nullptr) {
=======
// Calcula a quantidade de arestas (considerando grafo não-direcionado)
int Rede::QuantidadeArestas() const {
    int numArestas = 0;
    for (int i = 0; i < numArmazens; ++i) {
        Lista* vizinho = adjacencias[i];
        while (vizinho != nullptr) {
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
            numArestas++;
            vizinho = vizinho->proximo;
        }
    }
    return numArestas / 2; // Divide por 2 para grafos não-direcionados
}

<<<<<<< HEAD
int Rede::GrauMinimo(Rede* rede) {
    int grauMin = 0;
    int count = 0;
    Lista* aux = rede->nos;

    while(aux != nullptr) {
        Lista* no = aux->valorLista;
        int numArestas = 0;

        while (no != nullptr) {
            numArestas++;
            if(count != 0 && numArestas > grauMin) {
                break;
            }
            no = no->proximo;
=======
// Calcula o grau mínimo da rede
int Rede::GrauMinimo() const {
    if (numArmazens == 0) return 0;
    
    int grauMin = -1;
    for (int i = 0; i < numArmazens; ++i) {
        int grauAtual = 0;
        Lista* vizinho = adjacencias[i];
        while (vizinho != nullptr) {
            grauAtual++;
            vizinho = vizinho->proximo;
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
        }
        if (grauMin == -1 || grauAtual < grauMin) {
            grauMin = grauAtual;
        }
    }
    return grauMin == -1 ? 0 : grauMin;
}

<<<<<<< HEAD
int Rede::GrauMaximo(Rede* rede) {
    int grauMax = 0;
    int count = 0;
    Lista* aux = rede->nos;
=======
// Calcula o grau máximo da rede
int Rede::GrauMaximo() const {
    int grauMax = 0;
    for (int i = 0; i < numArmazens; ++i) {
        int grauAtual = 0;
        Lista* vizinho = adjacencias[i];
        while (vizinho != nullptr) {
            grauAtual++;
            vizinho = vizinho->proximo;
        }
        if (grauAtual > grauMax) {
            grauMax = grauAtual;
        }
    }
    return grauMax;
}
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc

// Retorna a lista de vizinhos com acesso O(1)
Lista* Rede::getVizinhos(int v) const {
    if (v < 0 || v >= numArmazens) return nullptr;
    return adjacencias[v];
}

// Imprime os vizinhos de um armazém
void Rede::ImprimeVizinhos(int v) const {
    if (v < 0 || v >= numArmazens) return;
    imprimeLista(adjacencias[v]);
}


// ----------------- Funções Auxiliares de Lista (C-style) -----------------

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

<<<<<<< HEAD
void Rede::InsereArmazem(Rede* rede) {
    int v = QuantidadeArmazens(rede);

    if(rede->nos == nullptr) {
        rede->nos = criaLista(TIPO_LISTA, nullptr, v);
    }
    else {
        adicionaItem(rede->nos, nullptr, v);
=======

// ----------------- Função de Cálculo de Rota (BFS) -----------------
// A lógica interna é a mesma, mas agora usa os métodos eficientes da Rede

int* calculaRota(const Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota) {
    bool* visitados = new bool[numArmazens];
    int* predecessores = new int[numArmazens];

    for (int i = 0; i < numArmazens; i++) {
        visitados[i] = false;
        predecessores[i] = -1;
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
    }

    Fila fila;
    fila.enfileirar(origem);
    visitados[origem] = true;

    while (!fila.vazia()) {
        int atual = fila.desenfileirar();
        if (atual == destino) break;

        // Acesso eficiente aos vizinhos
        Lista* vizinhos = rede.getVizinhos(atual);
        while (vizinhos != nullptr) {
            int vizinho_id = vizinhos->valorInteiro;
            if (!visitados[vizinho_id]) {
                visitados[vizinho_id] = true;
                predecessores[vizinho_id] = atual;
                fila.enfileirar(vizinho_id);
            }
            vizinhos = vizinhos->proximo;
        }
    }

    int* rota = nullptr;
    tamanhoRota = 0;

    if (predecessores[destino] != -1) {
        int cont = 0;
        int atual = destino;
        while (atual != -1) {
            cont++;
            atual = predecessores[atual];
        }

        tamanhoRota = cont;
        rota = new int[tamanhoRota];

<<<<<<< HEAD
void Rede::ImprimeVizinhos(int v) {
    Lista* aux = nos;
    int c = 0;
    while(c < v) {
        c++;
        aux = aux->proximo;
    }
    imprimeLista(aux->valorLista);
}

struct NoFila {
    int valor;
    NoFila* proximo;
};

class Fila {
private:
    NoFila* frente;
    NoFila* tras;
public:
    Fila() : frente(nullptr), tras(nullptr) {}
    
    void enfileirar(int valor) {
        NoFila* novo = new NoFila{valor, nullptr};
        if (tras) {
            tras->proximo = novo;
        } else {
            frente = novo;
        }
        tras = novo;
    }
    
    int desenfileirar() {
        if (frente == nullptr) return -1;
        NoFila* temp = frente;
        int valor = temp->valor;
        frente = frente->proximo;
        if (frente == nullptr) tras = nullptr;
        delete temp;
        return valor;
    }
    
    bool vazia() const {
        return frente == nullptr;
    }
    
    ~Fila() {
        while (!vazia()) {
            desenfileirar();
        }
    }
};

// Função auxiliar para obter vizinhos de um armazém
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

// Função para calcular rota usando BFS
int* Rede::calculaRota(Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota) {
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
=======
        atual = destino;
        for (int i = tamanhoRota - 1; i >= 0; i--) {
            rota[i] = atual;
            atual = predecessores[atual];
        }
    }

    delete[] visitados;
    delete[] predecessores;

    return rota; // ATENÇÃO: A responsabilidade de deletar a rota é do chamador!
}
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
