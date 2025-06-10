#include "../include/Armazem.h"
#include <iostream>

// --- Implementação da Classe Armazem ---

Armazem::Armazem() : id(-1), numSecoes(0), secoes(nullptr) {}

Armazem::Armazem(int id, int numTotalArmazens) : id(id), numSecoes(numTotalArmazens) {
    secoes = new Secao[numSecoes];
    for (int i = 0; i < numSecoes; ++i) {
        secoes[i].setIdArmazemDestino(i);
    }
}

Armazem::~Armazem() {
    delete[] secoes;
}

void Armazem::setId(int id) {
    this->id = id;
}

int Armazem::getId() const {
    return id;
}

void Armazem::armazenarPacote(Pacote* pacote) {
    if (pacote == nullptr) return;

    int proximoDestino = pacote->getProximoSalto();

    if (proximoDestino == -1) {
        return;
    }

    Secao* secaoAlvo = getSecaoPorDestino(proximoDestino);
    if (secaoAlvo != nullptr) {
        // --- DEBUG ---
        std::cerr << "[DEBUG] Armazem " << this->id << "::armazenarPacote ID=" << pacote->getId() << " na secao para " << proximoDestino << std::endl;
        // --- FIM DEBUG ---
        secaoAlvo->addPacote(pacote);
        pacote->setEstado(Pacote::ARMAZENADO);
    } else {
        std::cerr << "ERRO: Armazem " << this->id 
                  << " nao possui secao para o destino " << proximoDestino << std::endl;
    }
}

Secao* Armazem::getSecaoPorDestino(int idDestino) {
    if (idDestino < 0 || idDestino >= numSecoes) {
        return nullptr;
    }
    return &secoes[idDestino];
}

void Armazem::imprimePacotesArmazenados() const {
    std::cout << "--- Status do Armazem " << id << " ---" << std::endl;
    bool algumPacote = false;
    for (int i = 0; i < numSecoes; ++i) {
        if (!secoes[i].estaVazia()) {
            secoes[i].imprimeSecao();
            algumPacote = true;
        }
    }
    if (!algumPacote) {
        std::cout << "  Nenhum pacote armazenado." << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}