#include "../include/Pacote.h"
#include <cstring>
#include <iostream>

Pacote::Pacote(int id, const char *remetente, const char *destinatario, char tipo, int id_armz_orig, int id_armz_dest, double tempo_chegada)
    : id(id), tipo(tipo), id_armz_orig(id_armz_orig), id_armz_dest(id_armz_dest), rota(nullptr), tempo_chegada(tempo_chegada)
{
    this->remetente = new char[strlen(remetente) + 1];
    strcpy(this->remetente, remetente);

    this->destinatario = new char[strlen(destinatario) + 1];
    strcpy(this->destinatario, destinatario);
}

Pacote::~Pacote()
{
    delete[] remetente;
    delete[] destinatario;
    // Garante que a memória da lista encadeada da rota seja liberada.
    if (rota != nullptr) {
        deletaLista(rota);
    }
}

void Pacote::setRota(Lista* novaRota) {
    // Se já houver uma rota, delete-a antes de atribuir a nova.
    if (this->rota != nullptr) {
        deletaLista(this->rota);
    }
    this->rota = novaRota;
}

int Pacote::getId() const {
    return id;
}

int Pacote::getOrigem() const {
    return id_armz_orig;
}

int Pacote::getDestino() const {
    return id_armz_dest;
}

Lista* Pacote::getRota() const {
    return rota;
}

// --- IMPLEMENTAÇÃO DOS NOVOS MÉTODOS ---

int Pacote::getProximoSalto() const {
    // Se a rota existe e tem um próximo nó, retorne o valor dele.
    if (rota != nullptr && rota->proximo != nullptr) {
        return rota->proximo->valorInteiro;
    }
    // Caso contrário, não há próximo salto (já chegou ao destino ou rota inválida).
    return -1; 
}

void Pacote::avancarRota() {
    // Apenas avança o ponteiro da rota. O nó antigo não é deletado aqui,
    // pois a lista inteira será deletada no destrutor do Pacote.
    if (rota != nullptr) {
        // Para evitar vazamento de memória do nó 'cabeça' atual,
        // podemos deletá-lo antes de avançar.
        Lista* no_antigo = rota;
        rota = rota->proximo;
        no_antigo->proximo = nullptr; // Desconecta para não deletar o resto da lista
        delete no_antigo;
    }
}


void Pacote::imprimePacote() const {
    std::cout << "id:" << id << " Remetente: " << remetente << " Destinatário: " << destinatario << " tipo: " << tipo << " idOrigem: "
              << id_armz_orig << " idDestino: " << id_armz_dest << std::endl;
}

void Pacote::imprimeRota() const {
    imprimeRotas(rota);
    std::cout << "Próximo salto do pacote " << id <<": "<< this->getProximoSalto() << std::endl;
        
}