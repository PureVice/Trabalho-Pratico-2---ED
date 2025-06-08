#include "Rede.h"
#include "Fila.h"
#include "Armazem.h"
#include <iostream>
#include <cstdlib>

// ----------------- Implementação da Classe Rede -----------------

// Construtor: Aloca o array de listas de adjacência
Rede::Rede(int num_armazens_total) {
    this->capacidade = num_armazens_total;
    this->numArmazens = num_armazens_total; // Assume que todos serão usados
    this->adjacencias = new Lista*[capacidade];
    for (int i = 0; i < capacidade; ++i) {
        adjacencias[i] = nullptr; // Inicializa todas as listas como nulas
    }
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
    }
}

// Calcula a quantidade de arestas (considerando grafo não-direcionado)
int Rede::QuantidadeArestas() const {
    int numArestas = 0;
    for (int i = 0; i < numArmazens; ++i) {
        Lista* vizinho = adjacencias[i];
        while (vizinho != nullptr) {
            numArestas++;
            vizinho = vizinho->proximo;
        }
    }
    return numArestas / 2; // Divide por 2 para grafos não-direcionados
}

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
        }
        if (grauMin == -1 || grauAtual < grauMin) {
            grauMin = grauAtual;
        }
    }
    return grauMin == -1 ? 0 : grauMin;
}

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
int* converteListaParaArray(Lista* inicio, int& tamanhoArray_saida) {
    // 1ª Passagem: Contar o número de inteiros na lista
    int contador = 0;
    Lista* aux = inicio;
    while (aux != nullptr) {
        if (aux->tipo == TIPO_INTEIRO) {
            contador++;
        }
        aux = aux->proximo;
    }

    // Define o tamanho de saída e trata o caso de não haver inteiros
    tamanhoArray_saida = contador;
    if (contador == 0) {
        return nullptr;
    }

    // Alocar memória para o array. O chamador deve liberar com delete[].
    int* arrayResultado = new int[contador];

    // 2ª Passagem: Preencher o array com os valores
    aux = inicio; // Reinicia o ponteiro auxiliar para o início da lista
    int indice = 0;
    while (aux != nullptr) {
        if (aux->tipo == TIPO_INTEIRO) {
            arrayResultado[indice] = aux->valorInteiro;
            indice++;
        }
        aux = aux->proximo;
    }

    return arrayResultado;
}
void Rede::CriaSecoes(int v, Armazem* armazem) { //v é o ID do armazém
    int tam_saida = armazem->getNumDestnPossiveis();
    if (v < 0 || v >= numArmazens) return;
    int * arrayVizinhos = converteListaParaArray(adjacencias[v], tam_saida);
    for (int i = 0; i < tam_saida; i++)
    {
        
        int vizinho_id = arrayVizinhos[i];
        if (vizinho_id >= 0 && vizinho_id < numArmazens) {
            armazem->getSecao(i)->setIdArmazem(vizinho_id);
        } else {
            std::cerr << "ID de vizinho inválido: " << vizinho_id << std::endl;
        }
    }
    
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


// ----------------- Função de Cálculo de Rota (BFS) -----------------
// A lógica interna é a mesma, mas agora usa os métodos eficientes da Rede

int* calculaRota(const Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota) {
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