#ifndef SIMULADOR_H
#define SIMULADOR_H
#include "Rede.h"
#include "Armazem.h"
#include "Evento.h"
#include "Escalonador.h"
#include "Pacote.h"

// Define constants for time in hours
#define TEMPO_MANIPULACAO_PACOTE 1.0 // Tempo de manipulação de pacote na respectiva sala do armazém.
#define TEMPO_TRANSPORTE_ARMAZENS 3.0 // Tempo de transporte entre dois armazéns.

class Simulador {
public:
    Simulador(const char* arquivo); // Construtor que recebe o nome do arquivo de entrada
    ~Simulador();
    
    void executar();
    // void carregarDados(const char* arquivo); // This method is now integrated into the constructor

private:
    Rede* rede;
    int numArmazens;
    Armazem* armazens;
    Escalonador escalonador; // Make sure this is properly initialized
    double relogio; // Tempo atual da simulação em horas
    int pacotesAtivos; // Total de pacotes ativos na simulação
    void inicializarTransportes(); // Schedules the initial transport events
    void processarChegadaPacote(ChegadaPacoteEvento* evento); // Handles package arrival at an warehouse
    void processarTransporte(TransporteEvento* evento); // Handles package transport between warehouses
    void calcularERegistrarRota(Pacote* pacote); // Calculates and records the route for a package
    void registrarEntregaPacote(Pacote* pacote); // Registers package delivery
};

#endif