#ifndef TRANSPORTE_H
#define TRANSPORTE_H
//#include "Rede.h" não inclua
#include "Lista.h"
class Pacote{

    private:
        int id;
        char * remetente;
        char * destinatario;
        char tipo;
        int id_armz_orig;
        int id_armz_dest;
        Lista* rota; 

    public: 
        
        Pacote(int id, const char* remetente, const char* destinatario, char tipo, int id_armz_orig, int id_armz_dest);
        ~Pacote();
        int getId();
        int getOrigem();
        int getDestino();
        void imprimePacote();
        void imprimeRota();
        void setRota(Lista* rota);
    
};



#endif