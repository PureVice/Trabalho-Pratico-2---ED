#include "../include/Pacote.h"
#include <cstring>
#include <iostream>

Pacote::Pacote(int id, const char* remetente, const char* destinatario, char tipo, int id_armz_orig, int id_armz_dest)
    : id(id), tipo(tipo), id_armz_orig(id_armz_orig), id_armz_dest(id_armz_dest) {
    this->remetente = new char[strlen(remetente) + 1];
    strcpy(this->remetente, remetente);

    this->destinatario = new char[strlen(destinatario) + 1];
    strcpy(this->destinatario, destinatario);
}

Pacote::~Pacote() {
    delete[] remetente;
    delete[] destinatario;
}

int Pacote::getId(){

    return id;
}

void Pacote::imprimePacote(){
    std::cout << "id:" << id <<" Remetente: " << remetente <<" Destinatário: " << destinatario << " tipo: "<< tipo << " idOrigem: "
    <<id_armz_orig << " idDestino: "<< id_armz_dest<<std::endl;

}