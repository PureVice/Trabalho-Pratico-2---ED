// #include "../include/Rede.h"
// #include "../include/Fila.h"
// #include "../include/Pacote.h"
// #include "../include/Armazem.h"
// #include <iostream>
// #include <fstream>
// #include <cstdlib>

// using namespace std;

// int main(int argc, char **argv)
// {
//     if (argc < 2)
//     {
//         cerr << "Uso: " << argv[0] << " <arquivo.txt>" << endl;
//         return 1;
//     }
    
//     ifstream arquivo(argv[1]);
//     ifstream *a =  &arquivo;
//     if (!arquivo.is_open())
//     {
//         cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
//         return 1;
//     }

//     int num_armazens;
//     if (!(arquivo >> num_armazens))
//     {
//         cerr << "Erro ao ler número de armazéns" << endl;
//         return 1;
//     }

//     Armazem *v_armazens = new Armazem[num_armazens]; // vetor de armazéns

//     cout << "Criando rede com " << num_armazens << " armazéns..." << endl;

//     Rede rede(num_armazens);
//     for (int i = 0; i < num_armazens; i++)
//     {

//         int qtdArestas;
//         if (!(arquivo >> qtdArestas))
//         {
//             cerr << "Erro ao ler quantidade de arestas para armazém " << i << endl;
//             return 1;
//         }
//         v_armazens[i].setId(i);
//         v_armazens[i].setNumDestnPossiveis(num_armazens);

//         cout << "Armazém " << i << " com " << qtdArestas << " conexões..." << endl;

//         for (int j = 0; j < qtdArestas; j++)
//         {
//             int w;
//             if (!(arquivo >> w))
//             {
//                 cerr << "Erro ao ler conexão " << j << " do armazém " << i << endl;
//                 return 1;
//             }
//             if (w < 0 || w >= num_armazens)
//             {
//                 cerr << "Conexão inválida: " << w << endl;
//                 return 1;
//             }
//             cout << "  Conexão com armazém " << w << endl;
//             // i é o armazém atual, w é o armazém conectado
//             rede.InsereAresta(i, w);
//         }
//     }

//     // Validação básica da rede
//     if (rede.QuantidadeArmazens() != num_armazens)
//     {
//         cerr << "Erro: Quantidade de armazéns não corresponde!" << endl;
//         return 1;
//     }

//     // Teste da rede
//     cout << "\n=== Teste da Rede ===" << endl;
//     cout << "Armazens: " << rede.QuantidadeArmazens() << endl;
//     cout << "Arestas: " << rede.QuantidadeArestas() << endl;

//     for (int i = 0; i < num_armazens; i++)
//     {
//         cout << "Vizinhos do armazem " << i << ": ";
//         rede.ImprimeVizinhos(i);
//     }

//     int qtdPacotes = 0;
//     arquivo >> qtdPacotes;

//     cout << "\nQuantidade de pacotes a serem lidos: " << qtdPacotes << endl;

//     for (int i = 0; i < qtdPacotes; i++)
//     { // cria pacotes

//         int id = i;
//         char remetente[100];
//         char destinatario[100];
//         char tipo = 'J';
//         int id_armz_orig = -1;
//         int id_armz_dest = -1;
//         arquivo >> id_armz_orig >> id_armz_dest >> remetente >> destinatario >> tipo;
//         Pacote *p = new Pacote(id, remetente, destinatario, tipo, id_armz_orig, id_armz_dest);
//         p->imprimePacote();
//         int tam_rota = v_armazens[id_armz_orig].getNumDestnPossiveis();
//         Lista *rota = calculaRota2(rede, id_armz_orig, id_armz_dest, num_armazens, tam_rota);
//         p->setRota(rota);
//         cout << "Rota do pacote " << p->getId() << ": ";
//         p->imprimeRota();
//         for (int j = 0; j < num_armazens; j++)
//         {
//             Secao *end = v_armazens[j].getSecoes();
//             if (v_armazens[j].getId() == p->getOrigem())
//             {
//                 v_armazens[j].armazenarPacote(p);
//                 break;
//             }
//         }
//     }
//     cout << "\n\nimpressão dos pacotes armazenados nos armazéns:" << endl;
//     for (int i = 0; i < num_armazens; i++)
//     {
//         cout << "\n\n\n--------------Armazém " << v_armazens[i].getId() << ":" << endl;
//         Armazem::imprimePacotes(&v_armazens[i]);
//     }
//     arquivo.close();
//     return 0;
// }