#ifndef REDE_H
#define REDE_H
<<<<<<< HEAD

#include <cstdlib>

// Enumeração para tipos de variáveis
typedef enum {
  TIPO_INTEIRO,
  TIPO_LISTA
} TipoVariavel;

// Estrutura de lista encadeada
typedef struct s_lista {
  TipoVariavel tipo;
  int valorInteiro;
  struct s_lista* valorLista;
  struct s_lista* proximo;
} Lista;

=======

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

>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
class Rede {
private:
    Lista** adjacencias;          // Array de listas de adjacência (acesso O(1))
    int numArmazens;              // Número de armazéns na rede
    int capacidade;               // Capacidade máxima do array

    // Funções auxiliares para manipulação de listas
    Lista* criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro);
    void adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro) ;
    void imprimeLista(Lista* inicio) ;
    void deletaLista(Lista* inicio);

public:
    // Construtor agora recebe o total de armazéns para pré-alocar a memória
    Rede(int num_armazens_total);
    // Destrutor para liberar toda a memória alocada
    ~Rede();

<<<<<<< HEAD
    // Métodos de análise da rede
    int QuantidadeArmazens(Rede* rede) ;
    int QuantidadeArestas(Rede* rede) ;
    int GrauMinimo(Rede* rede) ;
    int GrauMaximo(Rede* rede) ;

    // Métodos de modificação da rede
    void InsereArmazem(Rede* rede);
    void InsereAresta(int v, int w);
    
    // Método de visualização
    void ImprimeVizinhos(int v) ;
    
    // Método de acesso
    Lista* getNos()  { return nos; }
    
    // Método para cálculo de rotas
    int* calculaRota(Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota);
=======
    // Insere uma aresta de 'v' para 'w'. Agora é um método da classe.
    void InsereAresta(int v, int w);

    // Métodos para obter as métricas da rede (agora mais eficientes)
    int QuantidadeArmazens() const;
    int QuantidadeArestas() const;
    int GrauMinimo() const;
    int GrauMaximo() const;

    // Imprime os vizinhos de um dado armazém
    void ImprimeVizinhos(int v) const;

    // Retorna a lista de adjacências de um armazém (usado pelo BFS)
    Lista* getVizinhos(int v) const;
>>>>>>> 55cf70206dda4e4b39a4bbb50634858c85e01dcc
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