#ifndef ARMAZEM_H
#define ARMAZEM_H
#include "PilhaPacotes.h"
#include "Pacote.h"

// Estrutura de nó para pilha de pacotes

//armazem --> secoes --> pilha --> pacotes
class Armazem {
public:
    Armazem(int id, int numDestnPossiveis);
    ~Armazem();
    
    void armazenarPacote(int destino, Pacote* pacote, double tempoAtual);
    Pacote* recuperarPacote(int destino, double tempoAtual);
    bool temPacoteParaDestino(int destino) const;
    
    // Novo método para recuperar pacotes sem removê-los
    PilhaPacotes* getSecao(int destino) const { 
        if (destino >= 0 && destino < numDestnPossiveis) {
            return secoes[destino];
        }
        return nullptr;
    }
    
private:
    int id;
    int numDestnPossiveis;
    PilhaPacotes** secoes;
};


#endif // ARMAZEM_H
