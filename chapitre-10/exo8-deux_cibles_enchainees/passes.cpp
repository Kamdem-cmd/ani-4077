#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Cible {
    std::string nom;
    long long w, h;
};

struct Dessin {
    std::string nom;
    std::string mode;
};

struct Passe {
    std::string cible;
    long long charge;
    long long ecrit;
};

void resoudreScenario(const std::string& labelScen) {
    int T = 0;
    if (!(std::cin >> T)) return;

    std::map<std::string, std::pair<long long, long long>> targetSizes;
    for (int i = 0; i < T; ++i) {
        std::string nom;
        long long w, h;
        std::cin >> nom >> w >> h;
        targetSizes[nom] = {w, h};
    }

    int P = 0;
    std::cin >> P;
    std::vector<Dessin> dessins(P);
    for (int i = 0; i < P; ++i) {
        std::cin >> dessins[i].nom >> dessins[i].mode;
    }

    std::vector<Passe> passes;
    std::string currentTarget = "FENETRE";
    int changements = 0;

    int i = 0;
    while (i < P) {
        std::string tName = dessins[i].nom;
        std::string firstMode = dessins[i].mode;

        if (tName != currentTarget) {
            changements++;
            currentTarget = tName;
        }

        // Regrouper les dessins consécutifs sur la même cible
        while (i < P && dessins[i].nom == tName) {
            i++;
        }

        auto sz = targetSizes[tName];
        long long area = sz.first * sz.second;
        long long charge = (firstMode == "GARDE") ? (area * 4) : 0;
        long long ecrit = area * 4;

        passes.push_back({tName, charge, ecrit});
    }

    std::cout << "SCENARIO " << labelScen << "\n";
    long long totalLus = 0;
    long long totalEcrits = 0;

    for (size_t k = 0; k < passes.size(); ++k) {
        std::cout << "PASSE " << (k + 1) << " " << passes[k].cible 
                  << " CHARGE " << passes[k].charge 
                  << " ECRIT " << passes[k].ecrit << "\n";
        totalLus += passes[k].charge;
        totalEcrits += passes[k].ecrit;
    }

    std::cout << "PASSES " << passes.size() << "\n";
    std::cout << "CHANGEMENTS " << changements << "\n";
    std::cout << "LUS " << totalLus << "\n";
    std::cout << "ECRITS " << totalEcrits << "\n";
    std::cout << "TOTAL " << (totalLus + totalEcrits) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");
    resoudreScenario("C");

    return 0;
}