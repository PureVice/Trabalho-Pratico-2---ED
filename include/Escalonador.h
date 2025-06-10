#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Evento.h" // O Escalonador gerencia ponteiros para Eventos.

class Escalonador {
public:
    // Construtor pode receber uma capacidade inicial para a heap.
    Escalonador(int capacidadeInicial = 100);
    ~Escalonador();

    // Insere um novo evento na fila de prioridade.
    void Inserir(Evento* ev);

    // Remove e retorna o evento de maior prioridade (menor chave) da fila.
    Evento* Remover();

    // Verifica se a fila de prioridade está vazia.
    bool Vazio() const;

private:
    // --- Métodos auxiliares para a Heap ---
    int GetAncestral(int posicao);
    int GetSucessorEsq(int posicao);
    int GetSucessorDir(int posicao);

    // Métodos para manter a propriedade da Min-Heap.
    void HeapifyPorBaixo(int posicao);
    void HeapifyPorCima(int posicao);

    // Método para aumentar a capacidade da heap dinamicamente.
    void redimensionar();

    // --- Membros da classe ---
    int tamanho;
    int capacidade;
    Evento** data; // A heap é implementada como um array de ponteiros para Evento.
};

#endif // ESCALONADOR_H