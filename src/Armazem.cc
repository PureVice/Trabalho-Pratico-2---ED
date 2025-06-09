#include "../include/Armazem.h"
#include <iostream>

// -------------------- PilhaPacotes - (No changes needed here based on the new logic) --------------------

// -------------------- Armazem --------------------
Armazem::Armazem() : id(-1), numDestnPossiveis(0), secoes(nullptr) {}

Armazem::Armazem(int id, int numDestnPossiveis) : id(id), numDestnPossiveis(numDestnPossiveis)
{
    // The sections are allocated in setNumDestnPossiveis now.
    secoes = new Secao[numDestnPossiveis];
    for (int i = 0; i < numDestnPossiveis; ++i) {
        new (&secoes[i]) Secao(); // Call default constructor for each Secao
    }
}

Armazem::~Armazem()
{
    // The Secao objects themselves need to be destructed before deleting the array.
    // If Secao has dynamic memory, its destructor should handle it.
    // Since Secao has a PilhaPacotes pointer, its destructor should delete it.
    delete[] secoes; // This calls destructors for each Secao in the array
}

// Helper to find or create a section for a given destination ID
Secao* Armazem::findOrCreateSecao(int destino_id) {
    // First, try to find an existing section for this destination
    for (int i = 0; i < numDestnPossiveis; ++i) {
        if (secoes[i].getIdArmazem() == destino_id) {
            return &secoes[i];
        }
    }

    // If not found, find an uninitialized section (id_armazem == -1) and initialize it
    for (int i = 0; i < numDestnPossiveis; ++i) {
        if (secoes[i].getIdArmazem() == -1) { // Assuming -1 means uninitialized
            secoes[i].setIdArmazem(destino_id);
            std::cout << "Criando seção " << destino_id << " no armazém " << id << std::endl;
            return &secoes[i];
        }
    }
    // If all sections are initialized and none match, this is an error or requires resizing
    // For now, let's assume numDestnPossiveis is sufficient or handle error.
    std::cerr << "Erro: Não foi possível encontrar ou criar seção para o destino " << destino_id << " no armazém " << id << std::endl;
    return nullptr; // Or throw an exception
}


void Armazem::armazenarPacote(Pacote *pacote)
{
    // The package is stored in the section corresponding to its *next* destination.
    int proximo_salto = pacote->getProximoSalto(); 
    
    // If the package has reached its final destination, it shouldn't be stored in a section for transport.
    // This case should ideally be handled before calling armazenarPacote,
    // or by checking if proximo_salto is -1 and then routing to a "delivered" state.
    if (proximo_salto == -1) {
        // This means the package has arrived at its final destination.
        // The Simulador should call a "registerDelivery" method instead of storing it again.
        // For now, let's just print a warning if this happens unexpectedly.
        std::cerr << "Aviso: Pacote " << pacote->getId() << " chegou ao destino final, mas foi tentado armazenar. Destino: " << pacote->getDestino() << std::endl;
        return; 
    }

    Secao* secao_alvo = findOrCreateSecao(proximo_salto);
    if (secao_alvo != nullptr) {
        secao_alvo->addPacote(pacote);
        std::cout << "Pacote " << pacote->getId() << " armazenado na seção " << secao_alvo->getIdArmazem() << " do armazém " << id << std::endl;
    } else {
        std::cerr << "Falha ao armazenar pacote " << pacote->getId() << ": seção para o próximo salto " << proximo_salto << " não encontrada/criada." << std::endl;
    }
}

Pacote *Armazem::recuperarPacote(int destino)
{
    // This method should retrieve from the section whose ID matches 'destino'.
    Secao* secao_alvo = nullptr;
    for (int i = 0; i < numDestnPossiveis; ++i) {
        if (secoes[i].getIdArmazem() == destino) {
            secao_alvo = &secoes[i];
            break;
        }
    }

    if (secao_alvo != nullptr) {
        return secao_alvo->desempilhar(); // LIFO
    }
    return nullptr;
}

bool Armazem::temPacoteParaDestino(int destino) const
{
    Secao* secao_alvo = nullptr;
    for (int i = 0; i < numDestnPossiveis; ++i) {
        if (secoes[i].getIdArmazem() == destino) {
            secao_alvo = &secoes[i];
            break;
        }
    }
    if (secao_alvo != nullptr) {
        return !secao_alvo->vazia();
    }
    return false;
}

void Armazem::setId(int id)
{
    this->id = id;
}

int Armazem::getId()
{
    return id;
}

void Armazem::setNumDestnPossiveis(int numDestnPossiveis)
{
    if (secoes)
    {
        delete[] secoes;
    }
    this->numDestnPossiveis = numDestnPossiveis;
    secoes = new Secao[numDestnPossiveis]; 

    // Initialize section IDs to -1 to mark them as unused
    for (int i = 0; i < numDestnPossiveis; ++i)
    {
        secoes[i].setIdArmazem(-1); // Mark as unassigned
    }
}

int Armazem::getNumDestnPossiveis()
{
    return numDestnPossiveis;
}

void Armazem::imprimePacotes(Armazem *armazem)
{
    bool found_packages = false;
    for (int i = 0; i < armazem->numDestnPossiveis; ++i)
    {
        if (armazem->secoes[i].getIdArmazem() != -1 && !(armazem->secoes[i].vazia()))
        {
            armazem->secoes[i].imprimeSecao();
            found_packages = true;
        }
    }
    if (!found_packages) {
        std::cout << "  Nenhum pacote armazenado neste armazém." << std::endl;
    }
}