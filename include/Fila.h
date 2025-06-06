#ifndef FILA_H
#define FILA_H

// Definição de uma fila simples para BFS
class Fila {
private:
    struct NoFila {
        int valor;
        NoFila* proximo;
    };
    
    NoFila* frente;
    NoFila* tras;

public:
    Fila();
    ~Fila();
    
    void enfileirar(int valor);
    int desenfileirar();
    bool vazia() const;
};

#endif // FILA_H