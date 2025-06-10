#ifndef REDE_H
#define REDE_H

#include "Lista.h"
#include "Armazem.h"

// A classe Rede modela o grafo de conexões entre os armazéns.
class Rede
{
public:
    // Construtor que recebe o número total de armazéns para inicializar o grafo.
    Rede(int numArmazens);
    ~Rede();

    // Insere uma aresta (conexão) entre dois armazéns 'v' e 'w'.
    void InsereAresta(int v, int w);

    // Retorna a lista de vizinhos (adjacências) de um dado armazém.
    Lista* getVizinhos(int v) const;

    // --- Getters para propriedades do grafo ---
    int getNumArmazens() const;
    void Imprime() const; // Para depuração.

private:
    int numArmazens;     // Número de vértices (armazéns) no grafo.
    Lista** adjacencias; // Array de listas de adjacência.
};

// --- Funções Auxiliares ---

// Função que calcula a rota mais curta (em número de saltos) entre uma origem e um destino.
// Usa o algoritmo de Busca em Largura (BFS).
Lista* calculaRota(const Rede& rede, int origem, int destino);

#endif // REDE_H