#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

struct Bloc {
    std::string nom;
    std::string symbole;
    std::vector<std::string> macros;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int B = 0;
    if (!(std::cin >> B)) {
        B = 0;
    }

    std::vector<Bloc> blocs(B);
    for (int i = 0; i < B; ++i) {
        int k = 0;
        std::cin >> blocs[i].nom >> blocs[i].symbole >> k;
        blocs[i].macros.resize(k);
        for (int j = 0; j < k; ++j) {
            std::cin >> blocs[i].macros[j];
        }
    }

    int T = 0;
    if (!(std::cin >> T)) {
        T = 0;
    }

    int bons = 0;
    int mauvais = 0;

    for (int i = 0; i < T; ++i) {
        std::string cible, attendu;
        int d = 0;
        std::cin >> cible >> attendu >> d;

        std::unordered_set<std::string> macros_definies;
        for (int j = 0; j < d; ++j) {
            std::string m;
            std::cin >> m;
            macros_definies.insert(m);
        }

        std::string bloc_pris = "REFUS";
        std::string symbole_pris = "AUCUN";

        // Évaluation dans l'ordre de déclaration des blocs (#if / #elif)
        for (const auto& bloc : blocs) {
            bool est_pris = false;
            for (const auto& macro : bloc.macros) {
                if (macros_definies.count(macro) > 0) {
                    est_pris = true;
                    break;
                }
            }

            if (est_pris) {
                bloc_pris = bloc.nom;
                symbole_pris = bloc.symbole;
                break; // Premier bloc correspondant gagne
            }
        }

        std::string verdict = (bloc_pris == attendu) ? "BON BLOC" : "MAUVAIS BLOC";
        if (verdict == "BON BLOC") {
            bons++;
        } else {
            mauvais++;
        }

        std::cout << cible << " " << bloc_pris << " " << symbole_pris << " " << verdict << "\n";
    }

    std::cout << "BONS " << bons << "\n";
    std::cout << "MAUVAIS " << mauvais << "\n";

    return 0;
}