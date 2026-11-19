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

class Reserve {
public:
    std::vector<Case> cases;
    std::stack<uint64_t> libres;

    void creer(const std::string& nom) {
        uint64_t idx;
        if (!libres.empty()) {
            idx = libres.top();
            libres.pop();
        } else {
            idx = cases.size();
            cases.push_back({"", 1, false});
        }
        cases[idx].nom = nom;
        cases[idx].occupee = true;
    }

    bool detruire(uint64_t i, uint64_t g) {
        if (g == 0) return false;
        if (i >= cases.size()) return false;
        if (!cases[i].occupee) return false;
        if (cases[i].generation != g) return false;

        cases[i].occupee = false;
        cases[i].nom.clear();
        cases[i].generation++;
        libres.push(i);
        return true;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::map<std::string, Reserve> reserves;
    long long ignores = 0;

    for (int n = 0; n < N; ++n) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "CREER") {
            std::string type, nom;
            std::cin >> type >> nom;
            reserves[type].creer(nom);
        } else if (cmd == "DETRUIRE") {
            std::string type;
            uint64_t i, g;
            std::cin >> type >> i >> g;
            if (!reserves[type].detruire(i, g)) {
                ignores++;
            }
        }
    }

    const std::vector<std::string> ordreTypes = {"MAILLAGE", "NUANCEUR", "TEXTURE", "CIBLE"};
    long long fuites = 0;

    for (const auto& type : ordreTypes) {
        const auto& res = reserves[type];
        for (size_t i = 0; i < res.cases.size(); ++i) {
            if (res.cases[i].occupee) {
                std::cout << "FUITE " << type << " " << res.cases[i].nom << "\n";
                fuites++;
            }
        }
    }

    std::cout << "FUITES " << fuites << "\n";
    std::cout << "IGNORES " << ignores << "\n";

    return 0;
}