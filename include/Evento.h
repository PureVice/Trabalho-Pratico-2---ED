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
    long long getPrioridade() const;
    
private:
    // Ordem corrigida para corresponder à lista de inicialização
    // e evitar o warning -Wreorder.
    double tempo;
    TipoEvento tipo;
    long long prioridade;

protected:
    void calcularPrioridadeChegada(Pacote* pacote);
    void calcularPrioridadeTransporte(int origem, int destino);
};


class ChegadaPacoteEvento : public Evento {
public:
    ChegadaPacoteEvento(double tempo, Pacote* pacote, int armazemChegada);
    
    Pacote* getPacote() const;
    int getArmazemChegada() const;
    
private:
    Pacote* pacote;
    int armazemChegada;
};


class TransporteEvento : public Evento {
public:
    TransporteEvento(double tempo, int origem, int destino);
    
    int getOrigem() const;
    int getDestino() const;
    
private:
    int origem;
    int destino;
};

#endif // EVENTO_H