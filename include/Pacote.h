#ifndef PACOTE_H
#define PACOTE_H

class Pacote {
public: 
    enum Estado {
        NAO_POSTADO,
        CHEGADA_ESCALONADA,
        CHEGOU_NAO_ARMAZENADO,
        ARMAZENADO,
        ALOCADO_TRANSPORTE,
        ENTREGUE
    };
    
    Pacote(int id, const char* remetente, const char* destinatario, char tipo, 
           int id_armz_orig, int id_armz_dest, double tempoPostagem);
    ~Pacote();
    
    // Métodos existentes
    int getId() const { return id; }
    
    // Novos métodos
    Estado getEstado() const { return estado; }
    void setEstado(Estado novoEstado) { estado = novoEstado; }
    
    int getOrigem() const { return id_armz_orig; }
    int getDestino() const { return id_armz_dest; }
    
    void registrarTempoArmazenamento(double tempo) { tempoArmazenado += tempo; }
    void registrarTempoTransporte(double tempo) { tempoTransporte += tempo; }
    void registrarTempoEntrega(double tempo) { tempoEntrega = tempo; }
    
    int* getRota() const { return rota; }
    int getTamanhoRota() const { return tamanhoRota; }
    int getProximoArmazemRota() const;
    
    void setRota(int* novaRota, int tamanho) {
        delete[] rota;
        rota = new int[tamanho];
        for (int i = 0; i < tamanho; i++) {
            rota[i] = novaRota[i];
        }
        tamanhoRota = tamanho;
        indiceRotaAtual = 1; // Começa no primeiro armazém após a origem
    }
    
    void avancarRota() {
        if (indiceRotaAtual < tamanhoRota - 1) {
            indiceRotaAtual++;
        }
    }

private:
    int id;
    char* remetente;
    char* destinatario;
    char tipo;
    int id_armz_orig;
    int id_armz_dest;
    Estado estado;
    double tempoPostagem;
    double tempoArmazenado;
    double tempoTransporte;
    double tempoEntrega;
    int* rota;               // Array com IDs dos armazéns na rota
    int tamanhoRota;         // Tamanho do array rota
    int indiceRotaAtual = 0; // Índice atual na rota
};

#endif // PACOTE_H