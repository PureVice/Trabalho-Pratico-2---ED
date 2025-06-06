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