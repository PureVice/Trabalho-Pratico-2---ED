#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "Pacote.h"
#include "Secao.h"

// Estrutura de nó para pilha de pacotes

//armazem --> secoes --> pilha --> pacotes
class Armazem {
public:
    Armazem();
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