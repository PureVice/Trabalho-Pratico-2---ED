#include "../include/Simulador.h" // Incluir o simulador
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_entrada.txt>" << std::endl;
        return 1;
    }
    Simulador sim(argv[1]);
   
    

    std::cout << "Simulacao concluida." << std::endl;

    return 0;
}