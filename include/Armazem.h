#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "Pacote.h"

// Estrutura de nó para pilha de pacotes
struct NoPilha {
    Pacote* pacote;
    NoPilha* proximo;
};

class PilhaPacotes {
public:
    PilhaPacotes();
    ~PilhaPacotes();

    void empilhar(Pacote* pacote);
    Pacote* desempilhar();
    Pacote* topo();
    bool vazia() const;

private:
    NoPilha* topoPilha;
};

class Armazem {
public:
    Armazem(int id, int numDestnPossiveis);
    ~Armazem();

    void armazenarPacote(int destino, Pacote* pacote);
    Pacote* recuperarPacote(int destino);
    bool temPacoteParaDestino(int destino) const;

private:
    int id;
    int numDestnPossiveis; // número total de armazéns
    PilhaPacotes** secoes; // vetor de ponteiros para pilhas (um para cada armazém conectado)
};

#endif // ARMAZEM_H
