#ifndef REDE_H
#define REDE_H
#include "Armazem.h"
#include "Lista.h"
// Enum para o tipo de dado armazenado na lista genérica

class Rede
{
private:
    Lista **adjacencias; // Array de listas de adjacência (acesso O(1))
    int numArmazens;     // Número de armazéns na rede
    int capacidade;      // Capacidade máxima do array

public:
    // Construtor agora recebe o total de armazéns para pré-alocar a memória
    Rede(int num_armazens_total);
    // Destrutor para liberar toda a memória alocada
    ~Rede();

    // Insere uma aresta de 'v' para 'w'. Agora é um método da classe.
    void InsereAresta(int v, int w);

    // Métodos para obter as métricas da rede (agora mais eficientes)
    int QuantidadeArmazens() const;
    int QuantidadeArestas() const;
    int GrauMinimo() const;
    int GrauMaximo() const;
    int getNumArmazens() const { return numArmazens; }
    // Imprime os vizinhos de um dado armazém
    void ImprimeVizinhos(int v) const;
    void CriaSecoes(int v, Armazem *armazem);
    // Retorna a lista de adjacências de um armazém (usado pelo BFS)
    Lista *getVizinhos(int v) const;
};


int *calculaRota(const Rede &rede, int origem, int destino, int numArmazens, int &tamanhoRota);
Lista *calculaRota2(const Rede &rede, int origem, int destino, int numArmazens, int &tamanhoRota);
#endif // REDE_H