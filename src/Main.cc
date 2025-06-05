#include "Armazem.h"
#include "Rede.h"
#include "Simulador.h"
#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char** argv) {
    char* arquivo = argv[0];
    std::ifstream file(arquivo);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo de pacotes!" << std::endl;
        return;
    }
    Rede r = Rede();
    int numArmazens;
    file >> numArmazens;
    for(int i = 0; i < numArmazens; i++) {
        Armazem a = Armazem(i, numArmazens);
        r.InsereArmazem(&r);
        
    }

     
    int id;
    char remetente[100], destinatario[100], tipo;
    int origem, destino;
    double tempoPostagem;
    
    while (file >> id >> remetente >> destinatario >> tipo >> origem >> destino >> tempoPostagem) {
        Pacote* pacote = new Pacote(id, remetente, destinatario, tipo, origem, destino, tempoPostagem);
        
        
        //calcularERegistrarRota(pacote); <-resolver
        
        
        ChegadaPacoteEvento* evento = new ChegadaPacoteEvento(tempoPostagem, pacote, origem);
        //escalonador.insereEvento(evento); <-resolver
    }
    
    file.close();



    const int numArmazens = 5;
    

    Rede rede;
    for (int i = 0; i < numArmazens; i++) {
        rede.InsereArmazem(&rede);
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
    simulador.carregarDados("pacotes.txt"); // Carrega pacotes de arquivo
    simulador.executar();
    
    return 0;
}