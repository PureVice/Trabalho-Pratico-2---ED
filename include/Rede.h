#ifndef REDE_H
#define REDE_H

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

class Rede {
private:
    Lista* nos;

    // Funções auxiliares para manipulação de listas
    Lista* criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro);
    void adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro) ;
    void imprimeLista(Lista* inicio) ;
    void deletaLista(Lista* inicio);

public:
    // Construtor e destrutor
    Rede();
    ~Rede();

    // Métodos de análise da rede
    int QuantidadeArmazens() ;
    int QuantidadeArestas() ;
    int GrauMinimo() ;
    int GrauMaximo() ;

    // Métodos de modificação da rede
    void InsereArmazem();
    void InsereAresta(int v, int w);
    
    // Método de visualização
    void ImprimeVizinhos(int v) ;
    
    // Método de acesso
    Lista* getNos()  { return nos; }
    
    // Método para cálculo de rotas
    int* calculaRota(Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota);
};

#endif // REDE_H