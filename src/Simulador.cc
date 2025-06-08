#include "Simulador.h"
#include "Rede.h"
#include "Armazem.h"
#include <iostream>
#include <fstream>
#define TEMPO_MANIPULACAO = 1.0
#define TEMPO_TRANSPORTE = 3.0
using namespace std;
Simulador::Simulador(const char *arquivotxt)
{   


    Escalonador escalonador;
    this->escalonador = escalonador;
    std::ifstream arquivo(arquivotxt);
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << arquivotxt << std::endl;
        exit(EXIT_FAILURE);
    }

    int numArmazens;
    arquivo >> numArmazens;
    this->numArmazens = numArmazens;
    Rede * r = new Rede(numArmazens);
    this->rede = r;
    this->armazens = new Armazem[numArmazens];

    //-------constrói o grafo
    for (int i = 0; i < numArmazens; i++)
    {

        int qtdArestas;
        if (!(arquivo >> qtdArestas))
        {
            cerr << "Erro ao ler quantidade de arestas para armazém " << i << endl;
            return;
        }
        this->armazens[i].setId(i);
        this->armazens[i].setNumDestnPossiveis(numArmazens);

        cout << "Armazém " << i << " com " << qtdArestas << " conexões->->->" << endl;

        for (int j = 0; j < qtdArestas; j++)
        {
            int w;
            if (!(arquivo >> w))
            {
                cerr << "Erro ao ler conexão " << j << " do armazém " << i << endl;
                return;
            }
            if (w < 0 || w >= numArmazens)
            {
                cerr << "Conexão inválida: " << w << endl;
                return;
            }
            cout << "  Conexão com armazém " << w << endl;
            // i é o armazém atual, w é o armazém conectado
            this->rede->InsereAresta(i, w);
        }
    }

    // Validação básica da this->rede
    if (this->rede->QuantidadeArmazens() != numArmazens)
    {
        cerr << "Erro: Quantidade de armazéns não corresponde!" << endl;
        return;
    }

    // Teste 
    cout << "\n=== Teste da this->rede ===" << endl;
    cout << "Armazens: " << this->rede->QuantidadeArmazens() << endl;
    cout << "Arestas: " << this->rede->QuantidadeArestas() << endl;

    for (int i = 0; i < numArmazens; i++)
    {
        cout << "Vizinhos do armazem " << i << ": ";
        this->rede->ImprimeVizinhos(i);
    }
    //-------
    // chegada dos pacotes
    int qtdPacotes;
    arquivo >> qtdPacotes;

    for (int i = 0; i < qtdPacotes; i++)
    { // cria pacotes
        
        int id = i;
        char remetente[100];
        char destinatario[100];
        char tipo = 'J';
        int id_armz_orig = -1;
        int id_armz_dest = -1;
        double tempo_chegada = -1.0; // Valor padrão para tempo de chegada
        arquivo >> id_armz_orig >> id_armz_dest >> remetente >> destinatario >> tipo >> tempo_chegada;
        Pacote *p = new Pacote(id, remetente, destinatario, tipo, id_armz_orig, id_armz_dest, tempo_chegada);
        p->imprimePacote();
        int tam_rota = armazens[id_armz_orig].getNumDestnPossiveis();
        Lista *rota = calculaRota2(*rede, id_armz_orig, id_armz_dest, numArmazens, tam_rota);
        p->setRota(rota);
        cout << "Rota do pacote " << p->getId() << ": ";
        p->imprimeRota();

        /* antes inseríamos os pacotes com for, não importava a
        ordem de chegada, mas agora vamos armazenar os pacotes 
        de acordo com sua ordem de chegada:
        1 - criar o pacote
        2 - calcular a rota
        3 - criar evento de chegada
        4 - colocar o evento na fila de eventos
        
        */
        for (int j = 0; j < numArmazens; j++)
        {
            Secao *end = armazens[j].getSecoes();
            if (armazens[j].getId() == p->getOrigem())
            {
                armazens[j].armazenarPacote(p);
                break;
            }
        }
    }
    cout << "\n\nimpressão dos pacotes armazenados nos armazéns:" << endl;
    for (int i = 0; i < numArmazens; i++)
    {
        cout << "\n\n\n--------------Armazém " << armazens[i].getId() << ":" << endl;
        Armazem::imprimePacotes(&armazens[i]);
    }
    arquivo.close();
    return ;








}

Simulador::~Simulador()
{
  
    delete[] armazens; // Libera a memória alocada para os armazéns

}