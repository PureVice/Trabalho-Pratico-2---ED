#include <string> // Não esqueça de incluir a biblioteca <string>

class Pacote {
public:
    int id;
    std::string remetente; // Alterado para std::string
    std::string destinatario; // Alterado para std::string
    char tipo;
    int id_armz_orig;
    int id_armz_dest;

    // Construtor corrigido
    Pacote(int id, const std::string& remetente, const std::string& destinatario, char tipo, int id_armz_orig, int id_armz_dest) {
        this->id = id;
        this->remetente = remetente;       // std::string faz a cópia profunda automaticamente
        this->destinatario = destinatario; // std::string faz a cópia profunda automaticamente
        this->tipo = tipo;
        this->id_armz_orig = id_armz_orig;
        this->id_armz_dest = id_armz_dest;
    }

    // Se precisar de um construtor que aceite const char*, pode sobrecarregar:
    Pacote(int id, const char* remetente_c, const char* destinatario_c, char tipo, int id_armz_orig, int id_armz_dest)
        : Pacote(id, std::string(remetente_c), std::string(destinatario_c), tipo, id_armz_orig, id_armz_dest) {
        // Este construtor chama o construtor principal que usa std::string
    }

    // Outros métodos da classe...
};