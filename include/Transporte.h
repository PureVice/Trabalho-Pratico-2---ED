#ifndef TRANSPORTE_H
#define TRANSPORTE_H

class Transporte {
public:
    Transporte(int origem, int destino, double frequencia, int capacidade)
        : origem(origem), destino(destino), frequencia(frequencia), capacidade(capacidade) {}
    
    int getOrigem() const { return origem; }
    int getDestino() const { return destino; }
    double getFrequencia() const { return frequencia; }
    int getCapacidade() const { return capacidade; }
    
private:
    int origem;
    int destino;
    double frequencia; // Tempo entre viagens em horas
    int capacidade;    // Número máximo de pacotes por viagem
};

#endif // TRANSPORTE_H