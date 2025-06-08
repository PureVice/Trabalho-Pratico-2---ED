#ifndef SIMULADOR_H
#define SIMULADOR_H
#include "Rede.h"
#include "Armazem.h"
#include "Evento.h"
#include "Escalonador.h"
#include "Pacote.h"

class Simulador {
public:
    Simulador(const char* arquivo); // Construtor que recebe o nome do arquivo de entrada
    ~Simulador();
    
    void executar();
    void carregarDados(const char* arquivo); // Para carregar pacotes de arquivo
    
private:
    Rede* rede;
    int numArmazens;
    Armazem* armazens;
    Escalonador escalonador;
    double relogio; // Tempo atual da simulação em horas
    
    void inicializarTransportes();
    void processarChegadaPacote(ChegadaPacoteEvento* evento);
    void processarTransporte(TransporteEvento* evento);
    void calcularERegistrarRota(Pacote* pacote);
};

#endif 

