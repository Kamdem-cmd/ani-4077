#include <iostream>
#include <string>
#include <map>
#include <set>

// Genre de contexte
enum class Genre {
    ANCIEN,
    CORE
};

struct ContextInfo {
    Genre genre;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "ERREURS 0\n";
        std::cout << "COURANT AUCUN\n";
        std::cout << "PROFIL AUCUN\n";
        std::cout << "VIVANTS 0\n";
        return 0;
    }

    // État initial de la machine à états
    bool has_dc = false;
    bool has_format = false;
    bool wgl_loaded = false;
    bool gl_loaded = false;
    
    std::string current_ctx = "AUCUN";
    std::map<std::string, ContextInfo> contexts;

    int erreurs = 0;

    for (int i = 1; i <= n; ++i) {
        std::string cmd;
        std::cin >> cmd;

        std::string status = "OK";

        if (cmd == "GETDC") {
            has_dc = true;
        } 
        else if (cmd == "FORMAT") {
            if (!has_dc) {
                status = "ERREUR PAS DE DC";
            } else if (has_format) {
                status = "ERREUR FORMAT DEJA POSE";
            } else {
                has_format = true;
            }
        } 
        else if (cmd == "CREER") {
            std::string nom;
            std::cin >> nom;
            if (!has_format) {
                status = "ERREUR PAS DE FORMAT";
            } else if (contexts.find(nom) != contexts.end()) {
                status = "ERREUR NOM PRIS";
            } else {
                contexts[nom] = {Genre::ANCIEN};
            }
        } 
        else if (cmd == "CREER_ATTRIBS") {
            std::string nom;
            std::cin >> nom;
            if (!has_format) {
                status = "ERREUR PAS DE FORMAT";
            } else if (!wgl_loaded) {
                status = "ERREUR EXTENSION NON CHARGEE";
            } else if (contexts.find(nom) != contexts.end()) {
                status = "ERREUR NOM PRIS";
            } else {
                contexts[nom] = {Genre::CORE};
            }
        } 
        else if (cmd == "COURANT") {
            std::string nom;
            std::cin >> nom;
            if (nom == "AUCUN") {
                current_ctx = "AUCUN";
            } else {
                if (contexts.find(nom) == contexts.end()) {
                    status = "ERREUR CONTEXTE INCONNU";
                } else {
                    current_ctx = nom;
                }
            }
        } 
        else if (cmd == "CHARGER_WGL") {
            if (current_ctx == "AUCUN") {
                status = "ERREUR PAS DE CONTEXTE COURANT";
            } else {
                wgl_loaded = true;
            }
        } 
        else if (cmd == "CHARGER_GL") {
            if (current_ctx == "AUCUN") {
                status = "ERREUR PAS DE CONTEXTE COURANT";
            } else {
                gl_loaded = true;
            }
        } 
        else if (cmd == "DETRUIRE") {
            std::string nom;
            std::cin >> nom;
            if (contexts.find(nom) == contexts.end()) {
                status = "ERREUR CONTEXTE INCONNU";
            } else if (current_ctx == nom) {
                status = "ERREUR CONTEXTE COURANT";
            } else {
                contexts.erase(nom);
            }
        } 
        else if (cmd == "GL") {
            std::string fn;
            std::cin >> fn;
            if (current_ctx == "AUCUN" || !gl_loaded) {
                status = "PLANTAGE ADRESSE ZERO";
            }
        }

        if (status != "OK") {
            erreurs++;
        }

        std::cout << i << " " << status << "\n";
    }

    // Calcul du bilan
    std::string profil = "AUCUN";
    if (current_ctx != "AUCUN") {
        if (contexts[current_ctx].genre == Genre::CORE) {
            profil = "CORE";
        } else {
            profil = "COMPATIBILITE";
        }
    }

    std::cout << "ERREURS " << erreurs << "\n";
    std::cout << "COURANT " << current_ctx << "\n";
    std::cout << "PROFIL " << profil << "\n";
    std::cout << "VIVANTS " << contexts.size() << "\n";

    return 0;
}