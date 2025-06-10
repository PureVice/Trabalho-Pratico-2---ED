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