#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "Secao.h"
#include "Pacote.h"

// A classe Armazem representa um centro de distribuição na rede.
// Ele contém várias seções, cada uma destinada a um armazém vizinho.
class Armazem
{
public:
    Armazem();
    // Construtor que define o ID e o número de destinos possíveis.
    Armazem(int id, int numTotalArmazens);
    ~Armazem();

    // Armazena um pacote na seção correta, com base no seu próximo destino.
    void armazenarPacote(Pacote* pacote);
    
    // Encontra a seção correspondente a um ID de destino.
    Secao* getSecaoPorDestino(int idDestino);

    // --- Getters e Setters ---
    void setId(int id);
    int getId() const;

    // Para depuração: imprime todos os pacotes em todas as seções.
    void imprimePacotesArmazenados() const;

private:
    int id;                // Identificador único do armazém.
    int numSecoes;         // O número de seções (geralmente, o total de armazéns).
    Secao* secoes;         // Array de seções de armazenamento.
};

#endif // ARMAZEM_H