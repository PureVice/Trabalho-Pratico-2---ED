#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Evento.h" // Inclui a definição de Evento

class Escalonador {
public:
    Escalonador(int capacidadeInicial = 100); // Construtor com capacidade inicial
    ~Escalonador();

    void Inserir(Evento* ev); // Agora insere um ponteiro para Evento
    Evento* Remover();        // Agora remove e retorna um ponteiro para Evento

    bool Vazio() const;

private:
    int GetAncestral(int posicao);
    int GetSucessorEsq(int posicao);
    int GetSucessorDir(int posicao);

    void HeapifyPorBaixo(int posicao);
    void HeapifyPorCima(int posicao);

    void redimensionar(); // Para evitar estouro de capacidade

    int tamanho;
    int capacidade;
    Evento** data; // O heap agora é um array de ponteiros para Evento
};

#endif