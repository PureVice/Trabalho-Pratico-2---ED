#ifndef SECAO_H
#define SECAO_H
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

class Secao {
public:
    Secao();
    Secao(int id_armazem);
    ~Secao();
    void addPacote(Pacote* pacote);
    Pacote* desempilhar();
    Pacote* topo();
    bool vazia() const;
    int getIdArmazem() const;
    void setIdArmazem(int id);
    
private:
    int id_armazem;
    PilhaPacotes * pilhaPacotes; // Pilha de pacotes nesta seção
};




#endif