#include "../include/Secao.h"
#include <iostream>

// --- Implementação da Classe PilhaPacotes ---

PilhaPacotes::PilhaPacotes() : topoPilha(nullptr), tamanho(0) {}

PilhaPacotes::~PilhaPacotes() {
    // O destrutor da classe Pacote NÃO é chamado aqui, pois a pilha
    // apenas armazena ponteiros. A posse dos pacotes é do Simulador.
    while (!vazia()) {
        desempilhar(); // Apenas remove o nó, não deleta o pacote.
    }
}

void PilhaPacotes::empilhar(Pacote* pacote) {
    NoPilha* novo = new NoPilha{pacote, topoPilha};
    topoPilha = novo;
    tamanho++;
}

Pacote* PilhaPacotes::desempilhar() {
    if (vazia()) return nullptr;

    NoPilha* temp = topoPilha;
    Pacote* pacote = temp->pacote;
    topoPilha = topoPilha->proximo;
    
    delete temp; // Libera a memória do nó da pilha.
    tamanho--;
    return pacote;
}

Pacote* PilhaPacotes::topo() const {
    if (vazia()) return nullptr;
    return topoPilha->pacote;
}

bool PilhaPacotes::vazia() const {
    return topoPilha == nullptr;
}

int PilhaPacotes::getTamanho() const {
    return tamanho;
}

// Retorna um array com todos os pacotes na pilha.
Pacote** PilhaPacotes::getPacotes() const {
    if (vazia()) return nullptr;
    
    Pacote** arrayPacotes = new Pacote*[tamanho];
    NoPilha* atual = topoPilha;
    int i = 0;
    while(atual != nullptr) {
        arrayPacotes[i++] = atual->pacote;
        atual = atual->proximo;
    }
    return arrayPacotes;
}


void PilhaPacotes::imprimePilha() const {
    if (vazia()) {
        return;
    }
    NoPilha* atual = topoPilha;
    while (atual != nullptr) {       
        std::cout << " " << atual->pacote->getId();
        atual = atual->proximo;
    }
}

// --- Implementação da Classe Secao ---

Secao::Secao() : idArmazemDestino(-1) {
    pilhaPacotes = new PilhaPacotes();
}

Secao::~Secao() {
    delete pilhaPacotes;
}

void Secao::addPacote(Pacote* pacote) {
    pilhaPacotes->empilhar(pacote);
}

Pacote* Secao::desempilharPacote() {
    return pilhaPacotes->desempilhar();
}

PilhaPacotes* Secao::getPilha() {
    return pilhaPacotes;
}

bool Secao::estaVazia() const {
    return pilhaPacotes->vazia();
}

int Secao::getIdArmazemDestino() const {
    return idArmazemDestino;
}

void Secao::setIdArmazemDestino(int id) { 
    idArmazemDestino = id;
}

void Secao::imprimeSecao() const {
    std::cout << "  Secao para Destino (" << idArmazemDestino << "): [TOPO]";
    pilhaPacotes->imprimePilha();
    std::cout << " [BASE]" << std::endl;
}