#include "Rede.h"
#include "Fila.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo.txt>" << endl;
        return 1;
    }

    ifstream arquivo(argv[1]);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        return 1;
    }

    int num_armazens;
    if (!(arquivo >> num_armazens)) {
        cerr << "Erro ao ler número de armazéns" << endl;
        return 1;
    }

    cout << "Criando rede com " << num_armazens << " armazéns..." << endl;
    
    Rede rede;
    for (int i = 0; i < num_armazens; i++) {
        int qtdArestas;
        if (!(arquivo >> qtdArestas)) {
            cerr << "Erro ao ler quantidade de arestas para armazém " << i << endl;
            return 1;
        }
        
        cout << "Armazém " << i << " com " << qtdArestas << " conexões..." << endl;
        rede.InsereArmazem();
        
        for (int j = 0; j < qtdArestas; j++) {
            int w;
            if (!(arquivo >> w)) {
                cerr << "Erro ao ler conexão " << j << " do armazém " << i << endl;
                return 1;
            }
            if (w < 0 || w >= num_armazens) {
                cerr << "Conexão inválida: " << w << endl;
                return 1;
            }
            cout << "  Conexão com armazém " << w << endl;
            
            InsereAresta(&rede, i, w);
        }
    }
    arquivo.close();

    // Validação básica da rede
    if (rede.QuantidadeArmazens() != num_armazens) {
        cerr << "Erro: Quantidade de armazéns não corresponde!" << endl;
        return 1;
    }

    // Teste da rede
    cout << "\n=== Teste da Rede ===" << endl;
    cout << "Armazens: " << rede.QuantidadeArmazens() << endl;
    cout << "Arestas: " << rede.QuantidadeArestas() << endl;
    cout << "Grau minimo: " << rede.GrauMinimo() << endl;
    cout << "Grau maximo: " << rede.GrauMaximo() << endl;
    
    for (int i = 0; i < num_armazens; i++) {
        cout << "Vizinhos do armazem " << i << ": ";
        rede.ImprimeVizinhos(i);
    }

    return 0;
}