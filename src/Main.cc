#include "../include/Simulador.h"
#include <iostream>

// Ponto de entrada do programa.
int main(int argc, char **argv) {
    // Verifica se o argumento da linha de comando (arquivo de entrada) foi fornecido.
    if (argc < 2) {
        std::cerr << "ERRO: Arquivo de entrada não especificado." << std::endl;
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada.txt>" << std::endl;
        return 1; // Retorna um código de erro.
    }

    // Cria o objeto Simulador. O construtor já carrega os dados do arquivo.
    Simulador sim(argv[1]);
   
    // Executa a simulação.
    sim.executar();

    return 0; // Termina com sucesso.
}