#include "../include/Pacote.h"

Pacote::Pacote(int id, const char* remetente, const char* destinatario, char tipo, int id_armz_orig, int id_armz_dest) {
    this->id = id;
    this->remetente = remetente;
    this->destinatario = destinatario;
    this->tipo = tipo;
    this->id_armz_orig = id_armz_orig;
    this->id_armz_dest = id_armz_dest;
}