#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>

struct Case {
    std::string nom;
    uint64_t generation = 1;
    bool occupee = false;
};

struct Poignee {
    size_t index = 0;
    uint64_t generation = 0;
};

class Reserve {
public:
    bool checkGen;
    std::vector<Case> cases;
    std::stack<size_t> libres;

    explicit Reserve(bool useGeneration) : checkGen(useGeneration) {}

    Poignee creer(const std::string& nom) {
        size_t idx;
        if (!libres.empty()) {
            idx = libres.top();
            libres.pop();
        } else {
            idx = cases.size();
            cases.push_back({"", 1, false});
        }
        cases[idx].nom = nom;
        cases[idx].occupee = true;
        return {idx, cases[idx].generation};
    }

    bool estBonne(Poignee p) const {
        if (p.generation == 0) return false;
        if (p.index >= cases.size()) return false;
        if (!cases[p.index].occupee) return false;
        if (checkGen && cases[p.index].generation != p.generation) return false;
        return true;
    }

    void detruire(Poignee p) {
        if (!estBonne(p)) return;
        cases[p.index].occupee = false;
        cases[p.index].nom.clear();
        cases[p.index].generation++;
        libres.push(p.index);
    }

    size_t vivantes() const {
        size_t count = 0;
        for (const auto& c : cases) {
            if (c.occupee) count++;
        }
        return count;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    Reserve mondeIndex(false);
    Reserve mondeGen(true);

    std::map<std::string, Poignee> poigneesIndex;
    std::map<std::string, Poignee> poigneesGen;
    std::map<std::string, std::string> nomCreation;

    long long fauxSilencieux = 0;
    long long perimeesDetectees = 0;

    for (int i = 1; i <= N; ++i) {
        std::string premier;
        std::cin >> premier;

        if (premier == "DETRUIRE") {
            std::string v;
            std::cin >> v;
            mondeIndex.detruire(poigneesIndex[v]);
            mondeGen.detruire(poigneesGen[v]);
        } else if (premier == "VOIR") {
            std::string v;
            std::cin >> v;

            std::string x = "RIEN";
            if (mondeIndex.estBonne(poigneesIndex[v])) {
                x = mondeIndex.cases[poigneesIndex[v].index].nom;
                if (x != nomCreation[v]) {
                    fauxSilencieux++;
                }
            }

            std::string y = "PERIMEE";
            if (mondeGen.estBonne(poigneesGen[v])) {
                y = mondeGen.cases[poigneesGen[v].index].nom;
            } else {
                perimeesDetectees++;
            }

            std::cout << "VOIR LIGNE " << i << " INDEX " << x << " GENERATION " << y << "\n";
        } else {
            // v = CREER nom
            std::string eq, op, nom;
            std::cin >> eq >> op >> nom;
            nomCreation[premier] = nom;
            poigneesIndex[premier] = mondeIndex.creer(nom);
            poigneesGen[premier] = mondeGen.creer(nom);
        }
    }

    std::cout << "FAUX SILENCIEUX " << fauxSilencieux << "\n";
    std::cout << "PERIMEES DETECTEES " << perimeesDetectees << "\n";
    std::cout << "VIVANTES INDEX " << mondeIndex.vivantes() << " GENERATION " << mondeGen.vivantes() << "\n";

    return 0;
}