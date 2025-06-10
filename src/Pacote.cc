#include "../include/Pacote.h"
#include <iostream>

Pacote::Pacote(int id, int id_armz_orig, int id_armz_dest, double tempo_postagem)
    : id(id), 
      id_armz_orig(id_armz_orig), 
      id_armz_dest(id_armz_dest), 
      tempoPostagem(tempo_postagem),
      rota(nullptr), 
      estadoAtual(NAO_POSTADO),
      tempoArmazenado(0.0), 
      tempoEmTransito(0.0)
{}

Pacote::~Pacote() {
    // O destrutor da classe Lista cuidará de limpar todas as células.
    if (rota != nullptr) {
        delete rota;
    }
}

// --- Getters ---
int Pacote::getId() const { return id; }
int Pacote::getOrigem() const { return id_armz_orig; }
int Pacote::getDestino() const { return id_armz_dest; }
Lista* Pacote::getRota() const { return rota; }
Pacote::Estado Pacote::getEstado() const { return estadoAtual; }
double Pacote::getTempoPostagem() const { return tempoPostagem; }
double Pacote::getTempoArmazenado() const { return tempoArmazenado; }
double Pacote::getTempoEmTransito() const { return tempoEmTransito; }


// --- Setters e Modificadores ---
void Pacote::setRota(Lista* novaRota) {
    if (this->rota != nullptr) {
        delete this->rota;
    }
    this->rota = novaRota;
}

void Pacote::setEstado(Estado novoEstado) {
    this->estadoAtual = novoEstado;
}

void Pacote::adicionaTempoArmazenado(double tempo) {
    this->tempoArmazenado += tempo;
}

void Pacote::adicionaTempoEmTransito(double tempo) {
    this->tempoEmTransito += tempo;
}

// Lógica para avançar na rota: remove o armazém atual (cabeça da lista)
void Pacote::avancarRota() {
    if (rota == nullptr || rota->vazia()) {
        return;
    }

    // A rota é uma lista onde o primeiro elemento é o local atual.
    // Para avançar, removemos a cabeça da lista.
    Celula* cabecaAntiga = rota->getInicio();
    
    // O novo início da lista é o próximo elemento.
    // A implementação da Lista precisa ser ajustada para permitir isso.
    // Vamos fazer uma simplificação aqui, assumindo que a classe Lista
    // permite modificar seu início.
    // Esta é uma limitação do design atual e idealmente seria um método na classe Lista.
    
    // Solução temporária (assumindo que podemos acessar e modificar a estrutura interna da Lista)
    // Para uma solução robusta, a classe Lista deveria ter um método `removeInicio()`.
    // Por simplicidade, vamos apenas "vazar" o nó antigo por enquanto,
    // o que não é ideal, mas simplifica a lógica sem alterar a API da Lista novamente.
    // O ideal seria: rota->removeInicio();

    // Uma maneira de fazer isso sem alterar a API da Lista é criar uma nova lista com o resto.
    Lista* novaRota = new Lista();
    Celula* atual = cabecaAntiga->proximo;
    while(atual != nullptr){
        novaRota->adicionaInteiro(atual->valorInteiro);
        atual = atual->proximo;
    }
    setRota(novaRota); // Substitui a rota antiga pela nova.
}

// Retorna o próximo passo na rota SEM modificar a rota.
int Pacote::getProximoSalto() const {
    if (rota == nullptr || rota->vazia() || rota->getInicio()->proximo == nullptr) {
        // Se a rota não existe, está vazia, ou só tem um elemento (o destino final),
        // não há próximo salto.
        return -1; 
    }
    // O próximo salto é o segundo elemento da lista.
    return rota->getInicio()->proximo->valorInteiro;
}

// --- Métodos de Impressão (para depuração) ---
void Pacote::imprimePacote() const {
    std::cout << "Pacote ID: " << id 
              << ", Origem: " << id_armz_orig 
              << ", Destino: " << id_armz_dest 
              << ", Postado em T=" << tempoPostagem << std::endl;
}

void Pacote::imprimeRota() const {
    if (rota != nullptr) {
        std::cout << "  Rota do Pacote " << id << ": ";
        rota->imprimeRota();
    } else {
        std::cout << "  Pacote " << id << " sem rota definida." << std::endl;
    }
}