#include "../include/Armazem.h"
#include <iostream>

// -------------------- PilhaPacotes --------------------

PilhaPacotes::PilhaPacotes() : topoPilha(nullptr) {}

PilhaPacotes::~PilhaPacotes() {
    while (!vazia()) {
        desempilhar();
    }
}

void PilhaPacotes::empilhar(Pacote* pacote) {
    NoPilha* novo = new NoPilha{pacote, topoPilha};
    topoPilha = novo;
}

Pacote* PilhaPacotes::desempilhar() {
    if (vazia()) return nullptr;
    NoPilha* temp = topoPilha;
    Pacote* pacote = temp->pacote;
    topoPilha = topoPilha->proximo;
    delete temp;
    return pacote;
}

Pacote* PilhaPacotes::topo() {
    if (vazia()) return nullptr;
    return topoPilha->pacote;
}

bool PilhaPacotes::vazia() const {
    return topoPilha == nullptr;
}

// -------------------- Armazem --------------------

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
