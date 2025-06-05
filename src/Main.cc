#include "Armazem.h"
#include "Rede.h"
#include "Simulador.h"
#include <iostream>

int main() {
    const int numArmazens = 5;
    
    // Cria rede de armazéns
    Rede rede;
    for (int i = 0; i < numArmazens; i++) {
        rede.InsereArmazem();
    }
    
    // Conecta os armazéns
    rede.InsereAresta(0, 1);
    rede.InsereAresta(0, 2);
    rede.InsereAresta(0, 4);
    rede.InsereAresta(1, 2);
    rede.InsereAresta(2, 3);
    rede.InsereAresta(3, 4);
    
    // Cria e executa simulador
    Simulador simulador(rede, numArmazens);
    simulador.carregarPacotes("pacotes.txt"); // Carrega pacotes de arquivo
    simulador.executar();
    
    return 0;
}