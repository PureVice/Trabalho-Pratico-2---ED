#ifndef EVENTO_H
#define EVENTO_H
#include "Pacote.h"
enum TipoEvento {
    CHEGADA_PACOTE,
    TRANSPORTE
};


class Evento {
public:
    Evento(double tempo, TipoEvento tipo) : tempo(tempo), tipo(tipo) {}
    virtual ~Evento() {}
    
    double getTempo() const { return tempo; }
    TipoEvento getTipo() const { return tipo; }
    
private:
    double tempo; // Tempo em horas desde a referência
    TipoEvento tipo;
};

class ChegadaPacoteEvento : public Evento {
public:
    ChegadaPacoteEvento(double tempo, Pacote* pacote, int armazem)
        : Evento(tempo, CHEGADA_PACOTE), pacote(pacote), armazem(armazem) {}
    
    Pacote* getPacote() const { return pacote; }
    int getArmazem() const { return armazem; }
    
private:
    Pacote* pacote;
    int armazem;
};

class TransporteEvento : public Evento {
public:
    TransporteEvento(double tempo, int origem, int destino, int capacidade)
        : Evento(tempo, TRANSPORTE), origem(origem), destino(destino), capacidade(capacidade) {}
    
    int getOrigem() const { return origem; }
    int getDestino() const { return destino; }
    int getCapacidade() const { return capacidade; }
    
private:
    int origem;
    int destino;
    int capacidade;
};

struct HeapEvento {
    Evento* evento;
    HeapEvento* esquerda;
    HeapEvento* direita;
    HeapEvento* pai;
};
#endif // EVENTO_H