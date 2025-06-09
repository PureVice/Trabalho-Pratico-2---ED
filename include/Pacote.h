#ifndef PACOTE_H
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
    double tempo_chegada_origem; // Time package arrived at the origin warehouse (or was posted)
    Lista* rota; 

    // Add new state variables for statistics
    double tempo_armazenado; // Total time stored in warehouses
    double tempo_em_transito; // Total time in transit
    double tempo_esperado_estadia; // Expected total time in the system (destination arrival time - original posting time)

public: 
    enum Estado{
        NAO_POSTADO,
        CHEGADA_ESCALONADA, // Scheduled to arrive at an warehouse
        CHEGOU_ARMAZEM,     // Arrived at an warehouse
        ARMAZENADO,         // Stored in a section of an warehouse
        ALOCADO_TRANSPORTE, // Allocated for transport
        EM_TRANSITO,        // Currently in transit between warehouses
        ENTREGUE            // Delivered to final destination
    } Estado_Atual; // Initialize to NAO_POSTADO in constructor

    Pacote(int id, const char* remetente, const char* destinatario, char tipo, int id_armz_orig, int id_armz_dest, double tempo_chegada = 0.0);
    ~Pacote();

    int getId() const;
    int getOrigem() const; // Original posting warehouse
    int getDestino() const; // Final destination warehouse
    Lista* getRota() const;
    
    // Getters for state and time
    Estado getEstadoAtual() const;
    double getTempoChegadaOrigem() const;
    double getTempoArmazenado() const;
    double getTempoEmTransito() const;
    double getTempoEsperadoEstadia() const;

    // Setters and update methods
    void setRota(Lista* rota);
    void avancarRota();
    void setEstadoAtual(Estado novoEstado);
    void addTempoArmazenado(double tempo);
    void addTempoEmTransito(double tempo);
    void setTempoEsperadoEstadia(double tempo);

    // Helper for next hop logic
    int getProximoSalto() const; // Returns the ID of the next warehouse in the route. Returns -1 if already at destination.

    void imprimePacote() const;
    void imprimeRota() const;
};

#endif // PACOTE_H