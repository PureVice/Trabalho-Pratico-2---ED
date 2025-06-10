#ifndef SECAO_H
#define SECAO_H

#include "Pacote.h"
#include <iostream> // Adicionado para std::ostream

// --- Estrutura do Nó da Pilha ---
// Um nó simples para a lista encadeada que forma a pilha.
struct NoPilha {
    Pacote* pacote;
    NoPilha* proximo;
};

// --- Classe PilhaPacotes ---
// Implementa a lógica LIFO (Last-In, First-Out) para os pacotes.
class PilhaPacotes {
public:
    PilhaPacotes();
    ~PilhaPacotes();

    void empilhar(Pacote* pacote); // Adiciona um pacote no topo.
    Pacote* desempilhar();         // Remove e retorna o pacote do topo.
    Pacote* topo() const;          // Apenas retorna o pacote do topo, sem remover.
    bool vazia() const;
    void imprimePilha(std::ostream& out = std::cout) const; // Assinatura alterada para debug
    int getTamanho() const;        // Retorna o número de pacotes na pilha.
    
    // Retorna uma cópia dos pacotes como um array para análise.
    // O chamador é responsável por deletar o array retornado.
    Pacote** getPacotes() const;

private:
    NoPilha* topoPilha;
    int tamanho;
};


// --- Classe Secao ---
// Representa uma seção de armazenamento dentro de um Armazém.
class Secao {
public:
    Secao();
    ~Secao();

    void addPacote(Pacote* pacote);
    Pacote* desempilharPacote();
    PilhaPacotes* getPilha(); // Permite acesso à pilha interna.
    bool estaVazia() const;
    int getIdArmazemDestino() const;
    void setIdArmazemDestino(int id);
    void imprimeSecao(std::ostream& out = std::cout) const; // Assinatura alterada para debug

private:
    int idArmazemDestino; // Para qual armazém esta seção envia pacotes.
    PilhaPacotes* pilhaPacotes;
};

#endif // SECAO_H