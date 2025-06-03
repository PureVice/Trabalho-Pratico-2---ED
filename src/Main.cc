#include "Test.h"

int main(int argc, char* argv[]) {
    Test* pTest = new Test();
    pTest->Print();
    return 0;
}

/*o sistema recebe pacotes enviados

esses pacostes devem ser guardados nos armazens
esses pocostes devem ser transportados entre os armazens
1)Um pacote chega inicialmente a qualquer armazém e é destinado a algum
outro armazém da rede.
2)Ao chegar a um armazém, o pacote recebe um número único
e é calculada a sua rota em termos de armazéns.

(As ligações entre os armazéns podem
ser vistas como um grafo, não direcionado, conhecido previamente e a rota pode ser vista
como uma lista encadeada.)



*/