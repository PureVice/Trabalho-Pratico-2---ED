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
    delete[] secoes;
}
void Armazem::criaSecao(int destino, int total_armazens)
{//3 -> 0
    int iter = 0;
    while (true)
    {

        if (secoes[iter].getIdArmazem() == -1 || secoes[iter].getIdArmazem() > total_armazens)
        {
            secoes[iter].setIdArmazem(destino);
            std::cout << "Criando seção " << destino << " no armazém " << id << std::endl;
            break;
            return;
        }
        iter++;
    }
    return;
}
void Armazem::armazenarPacote(Pacote *pacote)
{
    int destino = pacote->getProximoSalto();
    bool adicionado = false;
    int i;
    for (i = 0; i < numDestnPossiveis; i++)
    {
        if (secoes[i].getIdArmazem() == destino)
        {
            secoes[i].addPacote(pacote);
            adicionado = true;
            std::cout << "Pacote " << pacote->getId() << " armazenado na seção " << secoes[i].getIdArmazem() << " do armazém " << id << std::endl;
            return;
        }
    }
    if (adicionado == false)
    {
        criaSecao(destino, numDestnPossiveis); // segfault
        secoes[i].addPacote(pacote);
        std::cout << "Pacote " << pacote->getId() << " armazenado na seção " << secoes[i].getIdArmazem() << " do armazém " << id << std::endl;
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

void Armazem::imprimePacotes(Armazem *armazem)
{
    for (int i = 0; i < armazem->numDestnPossiveis; ++i)
    {
        if (!(armazem->secoes[i].vazia()))
        {
            armazem->secoes[i].imprimeSecao();
        }
    }
}