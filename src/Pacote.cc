#include "../include/Pacote.h"
#include <cstring>
#include <iostream>

Pacote::Pacote(int id, const char* remetente, const char* destinatario, char tipo, 
               int id_armz_orig, int id_armz_dest, double tempoPostagem)
    : id(id), tipo(tipo), id_armz_orig(id_armz_orig), id_armz_dest(id_armz_dest),
      estado(NAO_POSTADO), tempoPostagem(tempoPostagem), tempoArmazenado(0),
      tempoTransporte(0), tempoEntrega(0), rota(nullptr), tamanhoRota(0), indiceRotaAtual(0) {
    
    this->remetente = new char[strlen(remetente) + 1];
    strcpy(this->remetente, remetente);

    this->destinatario = new char[strlen(destinatario) + 1];
    strcpy(this->destinatario, destinatario);
}

Pacote::~Pacote() {
    delete[] remetente;
    delete[] destinatario;
    delete[] rota;
}

int Pacote::getProximoArmazemRota() const {
    if (rota && indiceRotaAtual < tamanhoRota - 1) {
        return rota[indiceRotaAtual + 1];
    }
    return -1; // Rota inválida ou já está no destino
}