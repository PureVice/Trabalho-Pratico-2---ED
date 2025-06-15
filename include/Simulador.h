#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Rede.h"
#include "Armazem.h"
#include "Evento.h"
#include "Escalonador.h"
#include "Pacote.h"
#include "Utils.h"

class Simulador {
public:
    Simulador(const char* arquivoEntrada);
    ~Simulador();
    
    void executar();

private:
    // Parâmetros da Simulação (lidos do arquivo)
    int capacidadeTransporte;
    double latenciaTransporte;
    double intervaloTransportes;
    double custoRemocao;

    
    Rede* rede;
    int numArmazens;
    Armazem** armazens;
    Escalonador escalonador;
    
    
    double relogio;
    int pacotesAtivos;
    Lista* logSaida;
    double tempoPrimeiroPacote;

    Pacote** todosOsPacotes; 
    int numTotalPacotes;     
    
    void carregarDados(const char* arquivoEntrada);
    void inicializarTransportes(double tempoInicial);
    
    void processarChegadaPacote(ChegadaPacoteEvento* evento);
    void processarTransporte(TransporteEvento* evento);
    
    // Métodos de logging e formatação para a saída.
    void registrarLog(double tempo, int idPacote, const char* mensagem);
    void imprimirLogs();
    void formatarId(int id, int largura, char* buffer);
};

#endif 