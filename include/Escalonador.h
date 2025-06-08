#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "Evento.h"

#include <cstdlib>


class Escalonador {
public:
    Escalonador();
    ~Escalonador();
    
    void insereEvento(Evento* evento);
    Evento* retiraProximoEvento();
    bool vazio() const;
    
private:
    HeapEvento* raiz;
    HeapEvento* ultimo;
    int tamanho;
    
    void subir(HeapEvento* no);
    void descer(HeapEvento* no);
    HeapEvento* encontrarPaiInsercao();
    HeapEvento* encontrarUltimo();
};

#endif 