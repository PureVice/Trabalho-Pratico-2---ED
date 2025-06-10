#ifndef LISTA_H
#define LISTA_H

#include <string> // O uso de string é permitido pelo enunciado.

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
    std::string valorString;
    
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
    void adicionaString(const std::string& valor);

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