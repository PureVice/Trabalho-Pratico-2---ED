#include "../include/Armazem.h"
#include <iostream>

// -------------------- PilhaPacotes --------------------

// -------------------- Armazem --------------------
Armazem::Armazem() : id(-1), numDestnPossiveis(0), secoes(nullptr) {}
Armazem::Armazem(int id, int numDestnPossiveis) : id(id), numDestnPossiveis(numDestnPossiveis)
{
    // secoes = new Secao[numDestnPossiveis];
    for (int i = 0; i < numDestnPossiveis; ++i)
    {
        // secoes[i] = Secao();
    }
}

Armazem::~Armazem()
{
    delete[] secoes; // Isso já chama destrutores automaticamente
    // REMOVA o loop manual de destruição!
}

void Armazem::armazenarPacote(int destino, Pacote *pacote)
{
    if (destino >= 0 && destino < numDestnPossiveis)
    {
        secoes[destino].addPacote(pacote);
        secoes[destino].setIdArmazem(destino); 
    }
}

Pacote *Armazem::recuperarPacote(int destino)
{
    if (destino >= 0 && destino < numDestnPossiveis)
    {
        return secoes[destino].desempilhar();
    }
    return nullptr;
}

bool Armazem::temPacoteParaDestino(int destino) const
{
    if (destino >= 0 && destino < numDestnPossiveis)
    {
        return !secoes[destino].vazia();
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
        delete[] secoes; // Só delete se já existir
    }
    this->numDestnPossiveis = numDestnPossiveis;
    secoes = new Secao[numDestnPossiveis]; // Construtor padrão obrigatório!

    // Inicialização correta:
    for (int i = 0; i < numDestnPossiveis; ++i)
    {
        // Se Secao não pode ser copiado, use placement new:
        new (&secoes[i]) Secao(i); // Construção in-place
    }
}
int Armazem::getNumDestnPossiveis()
{
    return numDestnPossiveis;
}

void Armazem::imprimePacotes(Armazem *armazem) {
    for (int i = 0; i < armazem->numDestnPossiveis; ++i) {
        std::cout << "Seção " << armazem->secoes[i].getIdArmazem() << " do Armazém " << armazem->id << ": ";
        if (armazem->secoes[i].vazia()) {
            std::cout << "Vazia" << std::endl;
        } else {
            Pacote *pacote = armazem->secoes[i].topo();
            while (pacote) {
                std::cout << pacote->getId() << " ";
                pacote = armazem->secoes[i].desempilhar(); 
            }
            std::cout << std::endl;
        }
    }
}