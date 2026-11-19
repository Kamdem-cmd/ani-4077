#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Objet {
    std::string nom;
    int P, T, M;
};

struct Compteur {
    int lastP = 0;
    int lastT = 0;
    int lastM = 0;
    long long liaisons = 0;
    long long inutiles = 0;

    void lieP(int p) {
        liaisons++;
        if (lastP == p) inutiles++;
        else lastP = p;
    }

    void lieT(int t) {
        liaisons++;
        if (lastT == t) inutiles++;
        else lastT = t;
    }

    void lieM(int m) {
        liaisons++;
        if (lastM == m) inutiles++;
        else lastM = m;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0, D = 0;
    if (!(std::cin >> N >> D)) return 0;

    if (N == 0) {
        std::cout << "ORDRE AUCUN\n";
        std::cout << "CREATION LIAISONS 0 INUTILES 0\n";
        std::cout << "REGROUPE LIAISONS 0 INUTILES 0\n";
        std::cout << "ECONOMIE AUCUNE\n";
        std::cout << "UTILES 0\n";
        return 0;
    }

    std::vector<Objet> objets(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> objets[i].nom >> objets[i].P >> objets[i].T >> objets[i].M;
    }

    // 1. Ordre de création
    Compteur cCreation;
    for (const auto& obj : objets) {
        cCreation.lieP(obj.P);
        cCreation.lieT(obj.T);
        cCreation.lieM(obj.M);
        if (D == 1) cCreation.lieM(0);
    }

    // 2. Regroupement par P puis par T (en ordre croissant)
    std::map<int, std::map<int, std::vector<Objet>>> groupes;
    for (const auto& obj : objets) {
        groupes[obj.P][obj.T].push_back(obj);
    }

    Compteur cRegroupe;
    std::vector<std::string> ordreNoms;

    for (const auto& [p, texMap] : groupes) {
        cRegroupe.lieP(p);
        for (const auto& [t, objList] : texMap) {
            cRegroupe.lieT(t);
            for (const auto& obj : objList) {
                ordreNoms.push_back(obj.nom);
                cRegroupe.lieM(obj.M);
                if (D == 1) cRegroupe.lieM(0);
            }
        }
    }

    std::cout << "ORDRE";
    for (const auto& nom : ordreNoms) {
        std::cout << " " << nom;
    }
    std::cout << "\n";

    std::cout << "CREATION LIAISONS " << cCreation.liaisons << " INUTILES " << cCreation.inutiles << "\n";
    std::cout << "REGROUPE LIAISONS " << cRegroupe.liaisons << " INUTILES " << cRegroupe.inutiles << "\n";

    long long e = cCreation.liaisons - cRegroupe.liaisons;
    long long p = (cCreation.liaisons == 0) ? 0 : (e * 100 / cCreation.liaisons);
    std::cout << "ECONOMIE " << e << " SUR " << cCreation.liaisons << " SOIT " << p << " %\n";

    long long utiles = cRegroupe.liaisons - cRegroupe.inutiles;
    std::cout << "UTILES " << utiles << "\n";

    return 0;
}