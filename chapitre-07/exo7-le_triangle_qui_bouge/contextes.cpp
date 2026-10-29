#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Fenetre {
    std::string nom;
    std::string couleur_affichee;
    int effacements;
};

struct Contexte {
    std::string nom;
    size_t fenetre_idx;
    std::string couleur_clear;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "IGNORES 0\n";
        return 0;
    }

    std::vector<Fenetre> fenetres;
    std::map<std::string, Contexte> contextes;
    std::string courant = "AUCUN";
    int ignores = 0;

    for (int i = 0; i < n; ++i) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "CREER") {
            std::string ctx_nom, fen_nom;
            std::cin >> ctx_nom >> fen_nom;

            if (contextes.find(ctx_nom) != contextes.end()) {
                ignores++;
            } else {
                fenetres.push_back({fen_nom, "noir", 0});
                contextes[ctx_nom] = {ctx_nom, fenetres.size() - 1, "noir"};
            }
        } else if (cmd == "COURANT") {
            std::string ctx_nom;
            std::cin >> ctx_nom;

            if (ctx_nom == "AUCUN") {
                courant = "AUCUN";
            } else {
                if (contextes.find(ctx_nom) == contextes.end()) {
                    ignores++;
                } else {
                    courant = ctx_nom;
                }
            }
        } else if (cmd == "COULEUR") {
            std::string coul;
            std::cin >> coul;

            if (courant == "AUCUN") {
                ignores++;
            } else {
                contextes[courant].couleur_clear = coul;
            }
        } else if (cmd == "EFFACER") {
            if (courant == "AUCUN") {
                ignores++;
            } else {
                Contexte& ctx = contextes[courant];
                Fenetre& fen = fenetres[ctx.fenetre_idx];
                fen.couleur_affichee = ctx.couleur_clear;
                fen.effacements++;
            }
        }
    }

    for (const auto& fen : fenetres) {
        std::cout << fen.nom << " " << fen.couleur_affichee << " " << fen.effacements << "\n";
    }
    std::cout << "IGNORES " << ignores << "\n";

    return 0;
}