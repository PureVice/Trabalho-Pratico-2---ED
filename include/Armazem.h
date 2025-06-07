#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "Pacote.h"
#include "Secao.h"

// Estrutura de nó para pilha de pacotes

// armazem --> secoes --> pilha --> pacotes
class Armazem
{
public:
    Armazem();
    Armazem(int id, int numDestnPossiveis);
    ~Armazem();

    void armazenarPacote(Pacote *pacote);
    Pacote *recuperarPacote(int destino);
    bool temPacoteParaDestino(int destino) const;
    void setId(int id);
    int getId();
    Secao *getSecao(int index) { return &secoes[index]; }
    Secao *getSecoes() { return secoes; }
    void setNumDestnPossiveis(int numDestnPossiveis);
    int getNumDestnPossiveis();
    static void imprimePacotes(Armazem * armazem);
    void criaSecao(int destino);
private:
    int id;
    int numDestnPossiveis; // número total de armazéns
    Secao *secoes;         // cada seção tem uma pilha
};

#endif // ARMAZEM_H