#ifndef EVENTO_H
#define EVENTO_H
#include "Pacote.h"

enum TipoEvento {
    CHEGADA_PACOTE,
    TRANSPORTE
};

class Evento {
public:
    Evento(double tempo, TipoEvento tipo);
    virtual ~Evento();
    
    double getTempo() const;
    TipoEvento getTipo() const;
    
private:
    double tempo; // Tempo em horas desde a referência
    TipoEvento tipo;
};

class ChegadaPacoteEvento : public Evento {
public:
    ChegadaPacoteEvento(double tempo, Pacote* pacote, int armazem);
    
    Pacote* getPacote() const;
    int getArmazem() const;
    
private:
    Pacote* pacote;
    int armazem;
};

class TransporteEvento : public Evento {
public:
    TransporteEvento(double tempo, int origem, int destino, int capacidade);
    
    int getOrigem() const;
    int getDestino() const;
    int getCapacidade() const;
    
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