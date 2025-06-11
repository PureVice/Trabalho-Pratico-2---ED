#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Rede.h"
#include "Armazem.h"
#include "Evento.h"
#include "Escalonador.h"
#include "Pacote.h"
#include "Utils.h" // Para o log de saída.
// #include <string> // Removido

class Simulador {
public:
    // O construtor recebe o caminho do arquivo de entrada.
    Simulador(const char* arquivoEntrada);
    ~Simulador();
    
    // Inicia e executa a simulação até que não haja mais eventos.
    void executar();

private:
    // --- Parâmetros da Simulação (lidos do arquivo) ---
    int capacidadeTransporte;
    double latenciaTransporte;
    double intervaloTransportes;
    double custoRemocao;

    // --- Componentes do Sistema ---
    Rede* rede;
    int numArmazens;
    Armazem** armazens; // Usando array de ponteiros para controlar a criação.
    Escalonador escalonador;
    
    // --- Controle da Simulação ---
    double relogio;
    int pacotesAtivos; // Contador de pacotes ainda não entregues.
    Lista* logSaida; // Lista para armazenar as strings de log para a saída final.
    double tempoPrimeiroPacote;
    // --- Métodos Privados Auxiliares ---
    void carregarDados(const char* arquivoEntrada);
    void inicializarTransportes();
    
    // Métodos para processar os diferentes tipos de eventos.
    void processarChegadaPacote(ChegadaPacoteEvento* evento);
    void processarTransporte(TransporteEvento* evento);
    
    // Métodos de logging e formatação para a saída.
    // Assinaturas alteradas para remover std::string
    void registrarLog(double tempo, int idPacote, const char* mensagem);
    void imprimirLogs();
    void formatarId(int id, int largura, char* buffer);
};

#endif // SIMULADOR_H