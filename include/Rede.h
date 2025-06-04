#ifndef REDE_H
#define REDE_H
//grafo e lista
typedef enum {
  TIPO_INTEIRO,
  TIPO_LISTA
} TipoVariavel;

typedef struct s_lista {
  TipoVariavel tipo;
  int valorInteiro;
  struct s_lista* valorLista;
  struct s_lista* proximo;
} Lista;

Lista* criaLista(TipoVariavel tipo, Lista* valorLista, int valorInteiro);
void adicionaItem(Lista* inicio, Lista* valorLista, int valorInteiro);
void imprimeLista(Lista* inicio);
void deletaLista(Lista* inicio);


class Rede {
private:
    Lista* nos;

public:
    // Construtor e destrutor
    Rede();
    ~Rede();

    // Métodos de análise do rede
    int QuantidadeArmazens() const;
    int QuantidadeArestas() const;
    int GrauMinimo() const;
    int GrauMaximo() const;

    // Métodos de modificação do rede
    void InsereArmazem();
    void InsereAresta(int v, int w);
    
    // Método de visualização
    void ImprimeVizinhos(int v) const;
};

#endif // REDE_H