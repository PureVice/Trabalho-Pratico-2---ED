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

void Armazem::armazenarPacote(int destino, Pacote* pacote, double tempoAtual) {
    if (pacote == nullptr) {
        std::cerr << "Erro: Pacote nulo não pode ser armazenado." << std::endl;
        return;
    }
    if (pacote->getEstado() == Pacote::NAO_POSTADO) {
        pacote->setEstado(Pacote::CHEGADA_ESCALONADA);
        pacote->registrarTempoArmazenamento(tempoAtual);
    } else if (pacote->getEstado() == Pacote::CHEGADA_ESCALONADA) {
        pacote->setEstado(Pacote::ARMAZENADO);
    }
    pacote->registrarTempoArmazenamento(tempoAtual);
    if (destino >= 0 && destino < numDestnPossiveis) {
        secoes[destino]->empilhar(pacote);
    }
}

Pacote* Armazem::recuperarPacote(int destino, double tempoAtual) {
    if (destino < 0 || destino >= numDestnPossiveis) {
        std::cerr << "Erro: Destino inválido." << std::endl;
        return nullptr;
    }
    if (secoes[destino]->vazia()) {
        std::cerr << "Erro: Não há pacotes para o destino " << destino << "." << std::endl;
        return nullptr;
    }
    Pacote* pacote = secoes[destino]->topo();
    if (pacote->getEstado() == Pacote::ARMAZENADO) {
        pacote->setEstado(Pacote::ALOCADO_TRANSPORTE);
        pacote->registrarTempoTransporte(tempoAtual);
    }
    pacote->avancarRota(); // Avança para o próximo armazém na rota
    pacote->registrarTempoArmazenamento(tempoAtual);
    // Desempilha o pacote após a recuperação
    pacote = secoes[destino]->topo();
    if (pacote == nullptr) {
        std::cerr << "Erro: Pacote não encontrado após desempilhamento." << std::endl;
        return nullptr;
    }
    pacote->setEstado(Pacote::CHEGOU_NAO_ARMAZENADO);
    pacote->registrarTempoArmazenamento(tempoAtual);
    // Desempilha o pacote da seção
    pacote = secoes[destino]->desempilhar();
    if (pacote == nullptr) {
        std::cerr << "Erro: Pacote não encontrado após desempilhamento." << std::endl;
        return nullptr;
    }
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
