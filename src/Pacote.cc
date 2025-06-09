#include "../include/Pacote.h"
#include <cstring>
#include <iostream>

Pacote::Pacote(int id, const char *remetente, const char *destinatario, char tipo, int id_armz_orig, int id_armz_dest, double tempo_chegada)
    : id(id), tipo(tipo), id_armz_orig(id_armz_orig), id_armz_dest(id_armz_dest), rota(nullptr), 
      tempo_chegada_origem(tempo_chegada), // Changed from tempo_chegada to tempo_chegada_origem for clarity
      tempo_armazenado(0.0), tempo_em_transito(0.0), tempo_esperado_estadia(0.0),
      Estado_Atual(NAO_POSTADO) // Initialize state
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
    if (rota != nullptr) {
        deletaLista(rota);
    }
}

void Pacote::setRota(Lista* novaRota) {
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

Pacote::Estado Pacote::getEstadoAtual() const {
    return Estado_Atual;
}

double Pacote::getTempoChegadaOrigem() const {
    return tempo_chegada_origem;
}

double Pacote::getTempoArmazenado() const {
    return tempo_armazenado;
}

double Pacote::getTempoEmTransito() const {
    return tempo_em_transito;
}

double Pacote::getTempoEsperadoEstadia() const {
    return tempo_esperado_estadia;
}

void Pacote::setEstadoAtual(Estado novoEstado) {
    Estado_Atual = novoEstado;
}

void Pacote::addTempoArmazenado(double tempo) {
    tempo_armazenado += tempo;
}

void Pacote::addTempoEmTransito(double tempo) {
    tempo_em_transito += tempo;
}

void Pacote::setTempoEsperadoEstadia(double tempo) {
    tempo_esperado_estadia = tempo;
}

int Pacote::getProximoSalto() const {
    if (rota != nullptr && rota->proximo != nullptr) {
        return rota->proximo->valorInteiro; // Next hop is the head of the remaining route
    }
    return -1; // No more hops, likely at destination or invalid route
}

void Pacote::avancarRota() {
    if (rota != nullptr) {
        Lista* no_antigo = rota;
        rota = rota->proximo; // Move to the next node in the route
        no_antigo->proximo = nullptr; // Disconnect the old head to prevent full list deletion
        delete no_antigo; // Delete the old head node
    }
}

void Pacote::imprimePacote() const {
    std::cout << "id:" << id << " Remetente: " << remetente << " Destinatário: " << destinatario << " tipo: " << tipo << " idOrigem: "
              << id_armz_orig << " idDestino: " << id_armz_dest << " Chega na origem em: "<< tempo_chegada_origem << " Estado: "<< Estado_Atual << std::endl ;
}

void Pacote::imprimeRota() const {
    imprimeRotas(rota);
    std::cout << "Próximo salto do pacote " << id <<": "<< this->getProximoSalto() << std::endl;
}