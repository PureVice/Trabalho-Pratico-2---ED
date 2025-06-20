#ifndef LISTA_H
#define LISTA_H

// O uso de string foi removido para atender aos requisitos.

// Enum para identificar o tipo de dado em cada célula da lista.
enum TipoVariavel
{
    TIPO_INTEIRO,
    TIPO_STRING
};

// A Célula (ou nó) da lista encadeada.
struct Celula
{
    // A célula pode armazenar um dos seguintes tipos de dados.
    int valorInteiro;
    char* valorString; // Alterado de std::string para char*
    
    TipoVariavel tipo;
    Celula *proximo;
};

// A classe Lista que gerencia as Células.
class Lista
{
public:
    Lista();
    ~Lista();

    // Métodos para adicionar itens ao final da lista.
    void adicionaInteiro(int valor);
    void adicionaString(const char* valor); // Assinatura alterada

    // Métodos para verificar estado ou obter dados.
    bool vazia() const;
    Celula* getInicio() const; // Retorna um ponteiro para o início da lista.

    // Métodos para impressão, úteis para depuração.
    void imprime() const;
    void imprimeRota() const;

private:
    Celula* inicio;
    Celula* fim;
};

#endif // LISTA_H

#ifndef FILA_H
#define FILA_H

// --- Nó da Fila ---
// Estrutura interna para a lista encadeada da fila.
struct NoFila {
    int valor;
    NoFila* proximo;
};

// --- Classe Fila ---
// Implementação de uma Fila simples para uso no algoritmo de BFS.
class Fila {
public:
    Fila();
    ~Fila();
    
    void enfileirar(int valor); // Adiciona um item ao final da fila.
    int desenfileirar();        // Remove e retorna o item do início da fila.
    bool vazia() const;

private:
    NoFila* frente;
    NoFila* tras;
};

#endif // FILA_H