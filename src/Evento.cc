#include "../include/Evento.h"

Evento::Evento(double tempo, TipoEvento tipo) : tempo(tempo), tipo(tipo) {}

Evento::~Evento() {}

double Evento::getTempo() const {
    return tempo;
}

TipoEvento Evento::getTipo() const {
    return tipo;
}

ChegadaPacoteEvento::ChegadaPacoteEvento(double tempo, Pacote* pacote, int armazem)
    : Evento(tempo, CHEGADA_PACOTE), pacote(pacote), armazem(armazem) {}

Pacote* ChegadaPacoteEvento::getPacote() const {
    return pacote;
}

int ChegadaPacoteEvento::getArmazem() const {
    return armazem;
}

TransporteEvento::TransporteEvento(double tempo, int origem, int destino, int capacidade)
    : Evento(tempo, TRANSPORTE), origem(origem), destino(destino), capacidade(capacidade) {}

int TransporteEvento::getOrigem() const {
    return origem;
}

int TransporteEvento::getDestino() const {
    return destino;
}

int TransporteEvento::getCapacidade() const {
    return capacidade;
}