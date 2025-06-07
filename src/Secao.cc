#include "Secao.h"
#include <iostream>
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
void PilhaPacotes::imprimePilha() const {
    if (vazia()) {
        //std::cout << "Pilha vazia!" << std::endl;
        return;
    }
    
    NoPilha* atual = topoPilha;
    
    while (atual != nullptr) {       
        
        std::cout << " " << atual->pacote->getId();
        
        atual = atual->proximo;
    }
    std::cout << std::endl;
}

//SECAO
Secao::Secao() : id_armazem(-1), pilhaPacotes(new PilhaPacotes()) {}
Secao::Secao(int id_armazem) : id_armazem(id_armazem), pilhaPacotes(new PilhaPacotes()) {}
Secao::~Secao() {
    delete []pilhaPacotes;
}
void Secao::addPacote(Pacote* pacote) {
    pilhaPacotes->empilhar(pacote);
}

Pacote* Secao::desempilhar() {
    return pilhaPacotes->desempilhar();
}

Pacote* Secao::topo() {
    return pilhaPacotes->topo();
}

bool Secao::vazia() const {
    return pilhaPacotes->vazia();
}

int Secao::getIdArmazem() const {
    return id_armazem;
}

void Secao::setIdArmazem(int id) { 
    id_armazem = id;
}
void Secao::imprimeSecao() const {
    std::cout << "Seção (" << id_armazem << "): ";
    pilhaPacotes->imprimePilha();
}