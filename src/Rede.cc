#include "Rede.h"
#include "Fila.h"
#include "Armazem.h" // Incluído para Pacote (indiretamente via Armazem)
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
    // Verifica se 'w' já está na lista para evitar duplicatas em grafo não-direcionado
    Lista* current = adjacencias[v];
    while (current != nullptr) {
        if (current->valorInteiro == w) {
            return; // Já existe a aresta, não adiciona novamente
        }
        current = current->proximo;
    }

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

// Esta função não é mais necessária no contexto da Rede, pois Armazem
// já cuida da inicialização das suas seções com setNumDestnPossiveis.
// A rede apenas informa as adjacências.
void Rede::CriaSecoes(int v, Armazem* armazem) {
    // A lógica de criação de seções do armazém agora está em Armazem::setNumDestnPossiveis
    // com base no total de armazéns na rede.
    // Esta função da Rede não é mais relevante para a criação de seções em Armazem.
    std::cerr << "AVISO: Rede::CriaSecoes() foi chamada, mas não deve ser usada diretamente para inicializar seções do Armazem. Armazem::setNumDestnPossiveis já lida com isso." << std::endl;
}

// Funções Auxiliares de Lista (C-style) - Mantidas as originais
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


// Função de Cálculo de Rota (BFS) - Mantida a original
Lista* calculaRota2(const Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota) {
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

    Lista* rotaLista = nullptr;
    tamanhoRota = 0;

    // Reconstrói a rota do destino para a origem e inverte para ter origem -> destino
    if (predecessores[destino] != -1 || origem == destino) { // Adicionado origem == destino para rotas de 1 nó
        int atual = destino;
        // Construir lista encadeada (do destino para a origem)
        while (atual != -1) {
            Lista* novoNo = criaLista(TIPO_INTEIRO, nullptr, atual); // Usar criaLista
            novoNo->proximo = rotaLista; // Insere no início
            rotaLista = novoNo;          // Atualiza cabeça da lista
            atual = predecessores[atual];
            tamanhoRota++;
            if (atual == origem) { // Para incluir a origem no início da rota
                novoNo = criaLista(TIPO_INTEIRO, nullptr, atual);
                novoNo->proximo = rotaLista;
                rotaLista = novoNo;
                tamanhoRota++;
                break;
            }
        }
    }

    delete[] visitados;
    delete[] predecessores;

    return rotaLista; // Chamador deve liberar a memória da lista
}