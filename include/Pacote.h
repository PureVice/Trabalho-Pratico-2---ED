#ifndef PACOTE_H
#define PACOTE_H

#include "Lista.h" // Inclui a nova classe Lista

class Pacote {
public: 
    // Enum para os estados do pacote, conforme descrito no enunciado.
    enum Estado {
        NAO_POSTADO,
        CHEGADA_AGENDADA, // Mapeia para "Chegada escalonada a um armazém"
        ARMAZENADO,       // Mapeia para "Armazenado na seção"
        EM_TRANSITO,      // Mapeia para "Removido da seção para transporte"
        ENTREGUE          // Mapeia para "Entregue"
    };

    Pacote(int id, int id_armz_orig, int id_armz_dest, double tempo_postagem);
    ~Pacote();

    // --- Getters ---
    int getId() const;
    int getOrigem() const;
    int getDestino() const;
    Lista* getRota() const;
    Estado getEstado() const;
    double getTempoPostagem() const;
    double getTempoArmazenado() const;
    double getTempoEmTransito() const;
    
    // --- Setters e Modificadores ---
    void setRota(Lista* rota);
    void avancarRota(); // Remove o nó atual da rota
    void setEstado(Estado novoEstado);
    void adicionaTempoArmazenado(double tempo);
    void adicionaTempoEmTransito(double tempo);

    // --- Métodos de Lógica ---
    // Retorna o ID do próximo armazém na rota. Retorna -1 se for o destino.
    int getProximoSalto() const; 
    
    // --- Métodos de Impressão (para depuração) ---
    void imprimePacote() const;
    void imprimeRota() const;

private:
    int id;
    int id_armz_orig;
    int id_armz_dest;
    double tempoPostagem; // Momento em que o pacote é postado no sistema. 
    Lista* rota; 
    Estado estadoAtual;

    // Variáveis para estatísticas, conforme requisitado. 
    double tempoArmazenado;
    double tempoEmTransito;
};

#endif // PACOTE_H