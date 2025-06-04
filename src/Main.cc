#include "Armazem.h"
#include <iostream>

int main() {
    Armazem a(0, 5);

    // Criando pacotes
    Pacote* p1 = new Pacote(1, "Alice", "Bob", 'C', 0, 1);
    Pacote* p2 = new Pacote(2, "Carol", "Dave", 'E', 0, 2);
    Pacote* p3 = new Pacote(3, "Eve", "Frank", 'R', 0, 1);

    // Armazenando
    a.armazenarPacote(1, p1);
    a.armazenarPacote(2, p2);
    a.armazenarPacote(1, p3);

    // Recuperando
    Pacote* r1 = a.recuperarPacote(1);
    std::cout << "Recuperou pacote com destino 1 (esperado: Eve -> Frank ou Alice -> Bob): " << (r1 != nullptr) << "\n";

    Pacote* r2 = a.recuperarPacote(2);
    std::cout << "Recuperou pacote com destino 2 (esperado: Carol -> Dave): " << (r2 != nullptr) << "\n";

    Pacote* r3 = a.recuperarPacote(1);
    std::cout << "Recuperou outro pacote com destino 1 (esperado: Alice -> Bob ou Eve -> Frank): " << (r3 != nullptr) << "\n";

    delete r1;
    delete r2;
    delete r3;

    return 0;
}


//o sistema recebe um pedido de envio de pacote
// pacote = new pacote
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