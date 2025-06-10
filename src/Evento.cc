#include "../include/Evento.h"

// --- Implementação da Classe Evento (Base) ---

Evento::Evento(double tempo, TipoEvento tipo) : tempo(tempo), tipo(tipo), prioridade(0) {}

Evento::~Evento() {}

double Evento::getTempo() const { return tempo; }
TipoEvento Evento::getTipo() const { return tipo; }
long long Evento::getPrioridade() const { return prioridade; }

void Evento::calcularPrioridadeChegada(Pacote* pacote) {
    long long tempoInt = static_cast<long long>(this->getTempo());
    // Use the packet ID directly for tie-breaking
    long long dadosEvento = pacote->getId(); 
    long long tipoEvento = 1; // Prioridade menor para ChegadaPacoteEvento para ser processado antes de TransporteEvento com o mesmo tempo
    // The packet ID should be a component of the priority to ensure deterministic ordering
    // Shift tempoInt by a large factor, then add packet ID, then type. This ensures
    // that tempo is the primary sort key, then packet ID, then event type.
    this->prioridade = tempoInt * 1000000000LL + dadosEvento * 10LL + tipoEvento; // Increased factor for tempoInt
}

void Evento::calcularPrioridadeTransporte(int origem, int destino) {
    long long tempoInt = static_cast<long long>(this->getTempo());
    // Combine origin and destination for unique tie-breaking
    long long dadosEvento = origem * 10000 + destino; // Larger factor for origin to distinguish from other events
    long long tipoEvento = 2; // Prioridade maior para TransporteEvento
    // Shift tempoInt by a large factor, then add combined origin/destination, then type.
    this->prioridade = tempoInt * 1000000000LL + dadosEvento * 10LL + tipoEvento; // Increased factor for tempoInt
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