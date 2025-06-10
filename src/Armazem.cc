#include "../include/Armazem.h"
#include <iostream>

// --- Implementação da Classe Armazem ---

Armazem::Armazem() : id(-1), numSecoes(0), secoes(nullptr) {}

Armazem::Armazem(int id, int numTotalArmazens) : id(id), numSecoes(numTotalArmazens) {
    // Aloca um array de Seções.
    secoes = new Secao[numSecoes];
    // Inicializa cada seção para um destino diferente.
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

// Encontra a seção correta com base no próximo destino do pacote e o armazena.
void Armazem::armazenarPacote(Pacote* pacote) {
    if (pacote == nullptr) return;

    // O próximo destino do pacote determina em qual seção ele será armazenado.
    int proximoDestino = pacote->getProximoSalto();

    // Se não há próximo salto, o pacote já deveria ter sido marcado como "ENTREGUE".
    if (proximoDestino == -1) {
        // Este caso não deveria ocorrer se a lógica do simulador estiver correta.
        // O pacote já está no seu destino final.
        return;
    }

    Secao* secaoAlvo = getSecaoPorDestino(proximoDestino);
    if (secaoAlvo != nullptr) {
        secaoAlvo->addPacote(pacote);
        pacote->setEstado(Pacote::ARMAZENADO);
    } else {
        // Erro: não deveria acontecer se o armazém foi inicializado corretamente.
        std::cerr << "ERRO: Armazem " << this->id 
                  << " não possui seção para o destino " << proximoDestino << std::endl;
    }
}

// Retorna um ponteiro para a seção que corresponde a um ID de destino.
Secao* Armazem::getSecaoPorDestino(int idDestino) {
    if (idDestino < 0 || idDestino >= numSecoes) {
        return nullptr; // ID de destino inválido.
    }
    // Como inicializamos as seções com IDs de 0 a N-1, podemos acessar diretamente.
    return &secoes[idDestino];
}

// Imprime o conteúdo de todas as seções não vazias (para depuração).
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