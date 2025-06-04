#include "Armazem.h"
#include "Rede.h"
#include <iostream>

int main()
{
    Armazem a(0, 5);

    // Criando pacotes
    Pacote *p1 = new Pacote(1, "Alice", "Bob", 'C', 0, 1);
    Pacote *p2 = new Pacote(2, "Carol", "Dave", 'E', 0, 2);
    Pacote *p3 = new Pacote(3, "Eve", "Frank", 'R', 0, 1);

    // Armazenando
    a.armazenarPacote(1, p1);
    a.armazenarPacote(2, p2);
    a.armazenarPacote(1, p3);

    // Recuperando
    Pacote *r1 = a.recuperarPacote(1);
    std::cout << "Recuperou pacote com destino 1 (esperado: Eve -> Frank ou Alice -> Bob): " << (r1 != nullptr) << "\n";

    Pacote *r2 = a.recuperarPacote(2);
    std::cout << "Recuperou pacote com destino 2 (esperado: Carol -> Dave): " << (r2 != nullptr) << "\n";

    Pacote *r3 = a.recuperarPacote(1);
    std::cout << "Recuperou outro pacote com destino 1 (esperado: Alice -> Bob ou Eve -> Frank): " << (r3 != nullptr) << "\n";

    delete r1;
    delete r2;
    delete r3;

    printf("\n\n\n\n\n\n\n");
    // Teste básico da classe Rede
    Rede g;

    // Insere 5 vértices (0 a 4)
    for (int i = 0; i < 5; i++)
    {
        g.InsereArmazem();
    }

// Teste da classe Rede
Rede r;

// Insere 5 armazéns (0 a 4)
for (int i = 0; i < 5; i++) {
    r.InsereArmazem();
}

// Insere arestas bidirecionais
r.InsereAresta(0, 1);
r.InsereAresta(0, 2);
r.InsereAresta(0, 4);  // Adicionado o que estava faltando
r.InsereAresta(1, 2);
r.InsereAresta(2, 3);
r.InsereAresta(3, 4);

// Verifica os resultados
std::cout << "=== Teste da Rede ===" << std::endl;
std::cout << "Armazens: " << r.QuantidadeArmazens() << " (esperado: 5)" << std::endl;
std::cout << "Arestas: " << r.QuantidadeArestas() << " (esperado: 6)" << std::endl;
std::cout << "Grau minimo: " << r.GrauMinimo() << " (esperado: 2)" << std::endl;
std::cout << "Grau maximo: " << r.GrauMaximo() << " (esperado: 3)" << std::endl;

std::cout << "\nVizinhos do armazem 0: ";
r.ImprimeVizinhos(0);  // Esperado: 1, 2, 4
std::cout << "Vizinhos do armazem 2: ";
r.ImprimeVizinhos(2);  // Esperado: 0, 1, 3
std::cout << "Vizinhos do armazem 4: ";
r.ImprimeVizinhos(4);  // Esperado: 0, 3
    return 0;
}

// o sistema recebe um pedido de envio de pacote
//  pacote = new pacote
/*o pacote chega qualquer arm_origem e tem que ir pra arm_destino
ele também recebe uma chave única -> int i = chave em for ?
ele também tem sua rota calculada -> calculaRota
ou seja:
p = pacote(..., arm_origem, arm_destino);
calculaRota(p) --> "em termos de armazens"


Secao{   ---- é uma pilha
    Pacote * pacotes
}
armazem{
Secao* secoes

}
*/