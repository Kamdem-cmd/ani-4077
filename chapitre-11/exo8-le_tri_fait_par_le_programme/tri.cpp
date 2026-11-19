#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

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

    long long utiles() const {
        return liaisons - inutiles;
    }
};

void resoudreScenario(const std::string& labelScen) {
    int N = 0, D = 0;
    std::string cle;
    if (!(std::cin >> N >> D >> cle)) return;

    std::vector<Objet> objets(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> objets[i].nom >> objets[i].P >> objets[i].T >> objets[i].M;
    }

    // 1. Version imbriquée
    std::map<int, std::map<int, std::vector<Objet>>> groupes;
    for (const auto& obj : objets) {
        groupes[obj.P][obj.T].push_back(obj);
    }

    Compteur cImbrique;
    for (const auto& [p, texMap] : groupes) {
        cImbrique.lieP(p);
        for (const auto& [t, objList] : texMap) {
            cImbrique.lieT(t);
            for (const auto& obj : objList) {
                cImbrique.lieM(obj.M);
                if (D == 1) cImbrique.lieM(0);
            }
        }
    }

    // 2. Version triée (tri stable)
    std::vector<Objet> tries = objets;
    std::stable_sort(tries.begin(), tries.end(), [&cle](const Objet& a, const Objet& b) {
        if (a.P != b.P) return a.P < b.P;
        if (a.T != b.T) return a.T < b.T;
        if (cle == "PTM") return a.M < b.M;
        return false;
    });

    Compteur cTrie;
    int prevP = -1;
    int prevT = -1;
    for (const auto& obj : tries) {
        if (obj.P != prevP) {
            cTrie.lieP(obj.P);
        }
        if (obj.P != prevP || obj.T != prevT) {
            cTrie.lieT(obj.T);
        }
        cTrie.lieM(obj.M);
        if (D == 1) cTrie.lieM(0);

        prevP = obj.P;
        prevT = obj.T;
    }

    std::cout << "SCENARIO " << labelScen << "\n";
    std::cout << "ORDRE";
    if (tries.empty()) {
        std::cout << " AUCUN";
    } else {
        for (const auto& obj : tries) {
            std::cout << " " << obj.nom;
        }
    }
    std::cout << "\n";

    std::cout << "IMBRIQUE LIAISONS " << cImbrique.liaisons << " UTILES " << cImbrique.utiles() << "\n";
    std::cout << "TRIE LIAISONS " << cTrie.liaisons << " UTILES " << cTrie.utiles() << "\n";
    std::cout << "MEME COMPTE " << (cImbrique.liaisons == cTrie.liaisons ? "OUI" : "NON") << "\n";
    std::cout << "UTILES GAGNEES " << (cImbrique.utiles() - cTrie.utiles()) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");
    resoudreScenario("C");

    return 0;
}