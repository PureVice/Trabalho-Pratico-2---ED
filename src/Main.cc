#include "Armazem.h"
#include "Rede.h"
#include <iostream>
#include <cstdlib>

// Definição de uma fila simples para BFS
struct NoFila {
    int valor;
    NoFila* proximo;
};

class Fila {
private:
    NoFila* frente;
    NoFila* tras;
public:
    Fila() : frente(nullptr), tras(nullptr) {}
    
    void enfileirar(int valor) {
        NoFila* novo = new NoFila{valor, nullptr};
        if (tras) {
            tras->proximo = novo;
        } else {
            frente = novo;
        }
        tras = novo;
    }
    
    int desenfileirar() {
        if (frente == nullptr) return -1;
        NoFila* temp = frente;
        int valor = temp->valor;
        frente = frente->proximo;
        if (frente == nullptr) tras = nullptr;
        delete temp;
        return valor;
    }
    
    bool vazia() const {
        return frente == nullptr;
    }
    
    ~Fila() {
        while (!vazia()) {
            desenfileirar();
        }
    }
};

// Função auxiliar para obter vizinhos de um armazém
Lista* getVizinhos(Rede& rede, int armazem) {
    Lista* aux = rede.getNos();
    int c = 0;
    while (c < armazem && aux != nullptr) {
        c++;
        aux = aux->proximo;
    }
    if (aux == nullptr) return nullptr;
    return aux->valorLista;
}

// Função para calcular rota usando BFS
int* calculaRota(Rede& rede, int origem, int destino, int numArmazens, int& tamanhoRota) {
    // Arrays para controle do BFS
    bool* visitados = new bool[numArmazens];
    int* predecessores = new int[numArmazens];
    
    for (int i = 0; i < numArmazens; i++) {
        visitados[i] = false;
        predecessores[i] = -1;
    }
    
    Fila fila;
    fila.enfileirar(origem);
    visitados[origem] = true;
    
    while (!fila.vazia()) {
        int atual = fila.desenfileirar();
        
        if (atual == destino) break;
        
        // Obtém vizinhos do armazém atual
        Lista* vizinhos = getVizinhos(rede, atual);
        while (vizinhos != nullptr) {
            int vizinho = vizinhos->valorInteiro;
            if (!visitados[vizinho]) {
                visitados[vizinho] = true;
                predecessores[vizinho] = atual;
                fila.enfileirar(vizinho);
            }
            vizinhos = vizinhos->proximo;
        }
    }
    
    // Reconstrói a rota
    int* rota = nullptr;
    tamanhoRota = 0;
    
    if (predecessores[destino] != -1) {
        // Conta o tamanho da rota
        int cont = 0;
        int atual = destino;
        while (atual != -1) {
            cont++;
            atual = predecessores[atual];
        }
        
        tamanhoRota = cont;
        rota = new int[tamanhoRota];
        
        // Preenche a rota na ordem correta
        atual = destino;
        for (int i = tamanhoRota - 1; i >= 0; i--) {
            rota[i] = atual;
            atual = predecessores[atual];
        }
    }
    
    delete[] visitados;
    delete[] predecessores;
    
    return rota;
}

int main(int argc, char* argv) {

    if(1 == 1){
        std::cout << "Opa" << std::endl;
    };

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
    
    // Testa a rede
    std::cout << "=== Teste da Rede ===" << std::endl;
    std::cout << "Armazens: " << rede.QuantidadeArmazens() << " (esperado: 5)" << std::endl;
    std::cout << "Arestas: " << rede.QuantidadeArestas() << " (esperado: 6)" << std::endl;
    std::cout << "Grau minimo: " << rede.GrauMinimo() << " (esperado: 2)" << std::endl;
    std::cout << "Grau maximo: " << rede.GrauMaximo() << " (esperado: 3)" << std::endl;
    
    std::cout << "\nVizinhos do armazem 0: ";
    rede.ImprimeVizinhos(0);
    std::cout << "Vizinhos do armazem 2: ";
    rede.ImprimeVizinhos(2);
    std::cout << "Vizinhos do armazem 4: ";
    rede.ImprimeVizinhos(4);
    
    // Cria armazéns individuais
    Armazem* armazens[numArmazens];
    for (int i = 0; i < numArmazens; i++) {
        armazens[i] = new Armazem(i, numArmazens);
    }
    
    // Cria pacotes
    Pacote* p1 = new Pacote(1, "Remetente A", "Destinatário X", 'E', 0, 3);
    Pacote* p2 = new Pacote(2, "Remetente B", "Destinatário Y", 'C', 1, 4);
    Pacote* p3 = new Pacote(3, "Remetente C", "Destinatário Z", 'R', 2, 0);
    
    // Processa os pacotes
    std::cout << "\n=== Processamento de Pacotes ===" << std::endl;
    
    // Pacote 1: Origem 0 -> Destino 3
    int tamanhoRota1;
    int* rota1 = calculaRota(rede, 0, 3, numArmazens, tamanhoRota1);
    std::cout << "Rota pacote 1: ";
    for (int i = 0; i < tamanhoRota1; i++) {
        std::cout << rota1[i] << (i < tamanhoRota1 - 1 ? " -> " : "");
    }
    std::cout << std::endl;
    armazens[0]->armazenarPacote(3, p1);
    std::cout << "Pacote 1 armazenado no armazém 0 (destino: 3)" << std::endl;
    
    // Pacote 2: Origem 1 -> Destino 4
    int tamanhoRota2;
    int* rota2 = calculaRota(rede, 1, 4, numArmazens, tamanhoRota2);
    std::cout << "Rota pacote 2: ";
    for (int i = 0; i < tamanhoRota2; i++) {
        std::cout << rota2[i] << (i < tamanhoRota2 - 1 ? " -> " : "");
    }
    std::cout << std::endl;
    armazens[1]->armazenarPacote(4, p2);
    std::cout << "Pacote 2 armazenado no armazém 1 (destino: 4)" << std::endl;
    
    // Pacote 3: Origem 2 -> Destino 0
    int tamanhoRota3;
    int* rota3 = calculaRota(rede, 2, 0, numArmazens, tamanhoRota3);
    std::cout << "Rota pacote 3: ";
    for (int i = 0; i < tamanhoRota3; i++) {
        std::cout << rota3[i] << (i < tamanhoRota3 - 1 ? " -> " : "");
    }
    std::cout << std::endl;
    armazens[2]->armazenarPacote(0, p3);
    std::cout << "Pacote 3 armazenado no armazém 2 (destino: 0)" << std::endl;
    
    // Simulação de recuperação de pacotes
    std::cout << "\n=== Recuperação de Pacotes ===" << std::endl;
    Pacote* pacoteRecuperado = armazens[0]->recuperarPacote(3);
    if (pacoteRecuperado) {
        std::cout << "Recuperado pacote " << pacoteRecuperado->getId()
                  << " do armazém 0, seção 3" << std::endl;
    }
    
    // Limpeza de memória
    for (int i = 0; i < numArmazens; i++) {
        delete armazens[i];
    }
    
    delete[] rota1;
    delete[] rota2;
    delete[] rota3;
    delete p1;
    delete p2;
    delete p3;
    
    return 0;
}