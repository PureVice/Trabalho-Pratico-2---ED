#include "../include/Rede.h"
#include "../include/Fila.h"
#include <iostream>

// --- Implementação da Classe Rede ---

Rede::Rede(int numArmazens) : numArmazens(numArmazens) {
    adjacencias = new Lista*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        adjacencias[i] = new Lista();
    }
}

Rede::~Rede() {
    for (int i = 0; i < numArmazens; ++i) {
        delete adjacencias[i];
    }
    delete[] adjacencias;
}

void Rede::InsereAresta(int v, int w) {
    if (v < 0 || v >= numArmazens || w < 0 || w >= numArmazens) return;
    adjacencias[v]->adicionaInteiro(w);
}

Lista* Rede::getVizinhos(int v) const {
    if (v < 0 || v >= numArmazens) return nullptr;
    return adjacencias[v];
}

int Rede::getNumArmazens() const {
    return numArmazens;
}

void Rede::Imprime() const {
    std::cout << "--- Topologia da Rede ---" << std::endl;
    for (int i = 0; i < numArmazens; ++i) {
        std::cout << "Armazem " << i << " -> ";
        adjacencias[i]->imprime();
    }
    std::cout << "-------------------------" << std::endl;
}

// --- Implementação da Função calculaRota (BFS) ---

Lista* calculaRota(const Rede& rede, int origem, int destino) {
    int numArmazens = rede.getNumArmazens();
    Lista* rota = new Lista();
    
    if (origem == destino) {
        rota->adicionaInteiro(origem);
        return rota;
    }

    bool* visitados = new bool[numArmazens];
    int* predecessores = new int[numArmazens];

    for (int i = 0; i < numArmazens; i++) {
        visitados[i] = false;
        predecessores[i] = -1;
    }

    Fila fila;
    fila.enfileirar(origem);
    visitados[origem] = true;

    bool destinoEncontrado = false;
    while (!fila.vazia() && !destinoEncontrado) {
        int atual = fila.desenfileirar();
        
        Lista* vizinhos = rede.getVizinhos(atual);
        Celula* vizinhoAtual = vizinhos->getInicio();

        while (vizinhoAtual != nullptr) {
            int idVizinho = vizinhoAtual->valorInteiro;
            if (!visitados[idVizinho]) {
                visitados[idVizinho] = true;
                predecessores[idVizinho] = atual;
                fila.enfileirar(idVizinho);

                if (idVizinho == destino) {
                    destinoEncontrado = true;
                    break;
                }
            }
            vizinhoAtual = vizinhoAtual->proximo;
        }
    }

    // Se o destino foi alcançado, reconstrói o caminho.
    if (visitados[destino]) {
        int contadorPassos = 0;
        int atualNaRota = destino;
        // Primeiro, conta quantos passos a rota possui
        while (atualNaRota != -1) {
            contadorPassos++;
            atualNaRota = predecessores[atualNaRota];
        }

        // Aloca um array temporário para armazenar a rota na ordem correta
        int* caminhoCorreto = new int[contadorPassos];
        
        atualNaRota = destino;
        // Preenche o array da última posição para a primeira (invertendo a ordem)
        for (int i = contadorPassos - 1; i >= 0; i--) {
            caminhoCorreto[i] = atualNaRota;
            atualNaRota = predecessores[atualNaRota];
        }
        
        // Adiciona os nós do array para a lista na ordem correta.
        for(int i = 0; i < contadorPassos; i++) {
            rota->adicionaInteiro(caminhoCorreto[i]);
        }
        
        delete[] caminhoCorreto; // Libera memória do array temporário
    }

    delete[] visitados;
    delete[] predecessores;

    return rota; // Retorna a rota (ou uma lista vazia se não houver caminho).
}