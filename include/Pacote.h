#ifndef PACOTE_H // Alterado de TRANSPORTE_H para PACOTE_H para consistência
#define PACOTE_H

#include "Lista.h"

class Pacote {
private:
    int id;
    char* remetente;
    char* destinatario;
    char tipo;
    int id_armz_orig;
    int id_armz_dest;
    double tempo_chegada; // Tempo de chegada do pacote
    Lista* rota; 
    enum Estado{
        
        NAO_POSTADO,
        CHEGADA_ESCALONADA,
        CHEGOU_ARMAZEM,
        ARMAZENADO,
        LOCAO_TRANSPORTE,
        ENTREGUE

    }Estado_Atual = NAO_POSTADO;

public: 
    Pacote(int id, const char* remetente, const char* destinatario, char tipo, int id_armz_orig, int id_armz_dest, double tempo_chegada = 0.0);
    ~Pacote();

    int getId() const;
    int getOrigem() const;
    int getDestino() const;
    Lista* getRota() const;
    
    // --- NOVOS MÉTODOS ---
    // Retorna o ID do próximo armazém na rota. Retorna -1 se já estiver no destino.
    int getProximoSalto() const;
    
    void avancarRota();

    void setRota(Lista* rota);
    void imprimePacote() const;
    void imprimeRota() const;
};

#endif // PACOTE_H