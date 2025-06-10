#include "../include/Evento.h"

// --- Implementação da Classe Evento (Base) ---

Evento::Evento(double tempo, TipoEvento tipo) : tempo(tempo), tipo(tipo), prioridade(0) {}

Evento::~Evento() {}

double Evento::getTempo() const { return tempo; }
TipoEvento Evento::getTipo() const { return tipo; }
long long Evento::getPrioridade() const { return prioridade; }

void Evento::calcularPrioridadeChegada(Pacote* pacote) {
    long long tempoInt = static_cast<long long>(this->getTempo());
    long long dadosEvento = pacote->getId();
    long long tipoEvento = 1;
    this->prioridade = tempoInt * 10000000LL + dadosEvento * 10LL + tipoEvento;
}

void Evento::calcularPrioridadeTransporte(int origem, int destino) {
    long long tempoInt = static_cast<long long>(this->getTempo());
    long long dadosEvento = origem * 1000 + destino; 
    long long tipoEvento = 2; 
    this->prioridade = tempoInt * 10000000LL + dadosEvento * 10LL + tipoEvento;
}

// --- Implementação de ChegadaPacoteEvento ---

ChegadaPacoteEvento::ChegadaPacoteEvento(double tempo, Pacote* pacote, int armazemChegada)
    : Evento(tempo, CHEGADA_PACOTE), pacote(pacote), armazemChegada(armazemChegada) {
    calcularPrioridadeChegada(pacote);
}

Pacote* ChegadaPacoteEvento::getPacote() const { return pacote; }
int ChegadaPacoteEvento::getArmazemChegada() const { return armazemChegada; }


// --- Implementação de TransporteEvento ---

TransporteEvento::TransporteEvento(double tempo, int origem, int destino)
    : Evento(tempo, TRANSPORTE), origem(origem), destino(destino) {
    calcularPrioridadeTransporte(origem, destino);
}

int TransporteEvento::getOrigem() const { return origem; }
int TransporteEvento::getDestino() const { return destino; }