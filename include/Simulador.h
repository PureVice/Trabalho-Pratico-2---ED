#include "Pacote.h"
#ifndef EVENTO_H
#define EVENTO_H

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

#endif // EVENTO_H


#ifndef ESCALONADOR_H
#define ESCALONADOR_H


#include <cstdlib>

// Estrutura para min-heap
struct HeapEvento {
    Evento* evento;
    HeapEvento* esquerda;
    HeapEvento* direita;
    HeapEvento* pai;
};

class Escalonador {
public:
    Escalonador();
    ~Escalonador();
    
    void insereEvento(Evento* evento);
    Evento* retiraProximoEvento();
    bool vazio() const;
    
private:
    HeapEvento* raiz;
    HeapEvento* ultimo;
    int tamanho;
    
    void subir(HeapEvento* no);
    void descer(HeapEvento* no);
    HeapEvento* encontrarPaiInsercao();
    HeapEvento* encontrarUltimo();
};

#endif 

#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Rede.h"
#include "Armazem.h"

class Simulador {
public:
    Simulador(Rede& rede, int numArmazens);
    ~Simulador();
    
    void executar();
    void carregarPacotes(const char* arquivo); // Para carregar pacotes de arquivo
    
private:
    Rede& rede;
    int numArmazens;
    Armazem** armazens;
    Escalonador escalonador;
    double relogio; // Tempo atual da simulação em horas
    
    void inicializarTransportes();
    void processarChegadaPacote(ChegadaPacoteEvento* evento);
    void processarTransporte(TransporteEvento* evento);
    void calcularERegistrarRota(Pacote* pacote);
};

#endif 

