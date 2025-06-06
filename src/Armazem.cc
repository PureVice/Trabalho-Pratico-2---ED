#include "../include/Armazem.h"
#include <iostream>

// -------------------- PilhaPacotes --------------------



// -------------------- Armazem --------------------
Armazem::Armazem() : id(-1), numDestnPossiveis(0), secoes(nullptr) {}
Armazem::Armazem(int id, int numDestnPossiveis) : id(id), numDestnPossiveis(numDestnPossiveis) {
    secoes = new PilhaPacotes*[numDestnPossiveis];
    for (int i = 0; i < numDestnPossiveis; ++i) {
        secoes[i] = new PilhaPacotes();
    }
}

Armazem::~Armazem() {
    for (int i = 0; i < numDestnPossiveis; ++i) {
        delete secoes[i];
    }
    delete[] secoes;
}

void Armazem::armazenarPacote(int destino, Pacote* pacote) {
    if (destino >= 0 && destino < numDestnPossiveis) {
        secoes[destino]->empilhar(pacote);
    }
}

Pacote* Armazem::recuperarPacote(int destino) {
    if (destino >= 0 && destino < numDestnPossiveis) {
        return secoes[destino]->desempilhar();
    }
    return nullptr;
}

bool Armazem::temPacoteParaDestino(int destino) const {
    if (destino >= 0 && destino < numDestnPossiveis) {
        return !secoes[destino]->vazia();
    }
    return false;
}