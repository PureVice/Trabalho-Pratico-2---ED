#ifndef REDE_H
#define REDE_H
#include "Armazem.h"

// Enum para o tipo de dado armazenado na lista genérica
enum TipoVariavel {
    TIPO_LISTA,
    TIPO_INTEIRO
};

// Estrutura genérica de nó para listas encadeadas
// Usada tanto para a lista de armazéns quanto para as listas de adjacência
struct Lista {
    int valorInteiro;
    Lista* valorLista;
    TipoVariavel tipo;
    Lista* proximo;
};

class Rede {
private:
    Lista** adjacencias;          // Array de listas de adjacência (acesso O(1))
    int numArmazens;              // Número de armazéns na rede
    int capacidade;               // Capacidade máxima do array

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

    // Imprime os vizinhos de um dado armazém
    void ImprimeVizinhos(int v) const;
    void CriaSecoes(int v, Armazem* armazem);
    // Retorna a lista de adjacências de um armazém (usado pelo BFS)
    Lista* getVizinhos(int v) const;
};

// --- Funções Auxiliares para Manipulação de Lista (C-style) ---
// (Mantidas conforme o código original, mas poderiam ser métodos privados/estáticos)

Lista* criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro);
void adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro);
void imprimeLista(Lista* inicio);
void deletaLista(Lista* inicio);

// --- Função de Roteamento (BFS) ---
// A assinatura foi mantida, mas agora opera sobre a nova estrutura de Rede.
int* calculaRota(const Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota);

#endif // REDE_H