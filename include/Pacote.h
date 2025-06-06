#ifndef TRANSPORTE_H
#define TRANSPORTE_H

class Pacote{

    private:
        int id;
        char * remetente;
        char * destinatario;
        char tipo;
        int id_armz_orig;
        int id_armz_dest;

    public: 
        
        Pacote(int id, const char* remetente, const char* destinatario, char tipo, int id_armz_orig, int id_armz_dest);
        ~Pacote();
        int getId();
        int getOrigem();
        int getDestino();
        void imprimePacote();
    
};



#endif