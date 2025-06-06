
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Rede.h"


using namespace std;
int main(int argc, char **argv)
{

    if (freopen(argv[1], "r", stdin) == nullptr)
    {
        cerr << "erro ao abrir o arquivo! " << argv[1] << std::endl;
        return 1;
    }
    int num_armazens;
    cin >> num_armazens;
    Rede *g = new Rede();

    for (int i = 0; i < num_armazens; i++)
    {
        int qtdArestas;
        cin >> qtdArestas;

        g->InsereArmazem(g);
        for (int j = 0; j < qtdArestas; j++)
        {
            int w; //i é o armazém atual, w é o armazém vizinho
            cin >> w;
            g->InsereAresta(i, w);
        }
    }
    for (int i = 0; i < num_armazens; i++)
    {
        cout << "Armazém " << i << ": ";
        //g->ImprimeVizinhos(i);
    }
    delete g;
    return 0;
}