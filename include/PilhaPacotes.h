#ifndef PILHA_PACOTE_H
#define PILHA_PACOTE_H
#include "Pacote.h"


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

#endif