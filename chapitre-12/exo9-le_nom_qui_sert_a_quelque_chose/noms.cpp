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

void resoudreScenario(const std::string& labelScen) {
    int N = 0;
    if (!(std::cin >> N)) return;

    std::vector<Case> cases;
    std::stack<uint64_t> libres;
    std::map<std::pair<uint64_t, uint64_t>, std::string> historiqueNoms;

    long long perimees = 0;
    long long inconnues = 0;

    std::cout << "SCENARIO " << labelScen << "\n";

    for (int n = 0; n < N; ++n) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "CREER") {
            std::string nom;
            std::cin >> nom;
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
            uint64_t gen = cases[idx].generation;

            historiqueNoms[{idx, gen}] = nom;
            std::cout << "CREER " << nom << " " << idx << " " << gen << "\n";
        } else if (cmd == "DETRUIRE") {
            uint64_t i, g;
            std::cin >> i >> g;
            if (g != 0 && i < cases.size() && cases[i].occupee && cases[i].generation == g) {
                std::cout << "DETRUIRE " << i << " " << g << " " << cases[i].nom << "\n";
                cases[i].occupee = false;
                cases[i].nom.clear();
                cases[i].generation++;
                libres.push(i);
            } else {
                std::cout << "DETRUIRE " << i << " " << g << " RIEN\n";
            }
        } else if (cmd == "VOIR") {
            uint64_t i, g;
            std::cin >> i >> g;
            if (g != 0 && i < cases.size() && cases[i].occupee && cases[i].generation == g) {
                std::cout << "VOIR " << i << " " << g << " " << cases[i].nom << "\n";
            } else {
                auto it = historiqueNoms.find({i, g});
                if (it != historiqueNoms.end()) {
                    std::string occupant = "VIDE";
                    if (i < cases.size() && cases[i].occupee) {
                        occupant = cases[i].nom;
                    }
                    std::cout << "VOIR " << i << " " << g << " PERIMEE " << it->second << " CASE " << occupant << "\n";
                    perimees++;
                } else {
                    std::cout << "VOIR " << i << " " << g << " INCONNUE\n";
                    inconnues++;
                }
            }
        }
    }

    std::cout << "PERIMEES " << perimees << "\n";
    std::cout << "INCONNUES " << inconnues << "\n";
    std::cout << "NOMS GARDES " << historiqueNoms.size() << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");
    resoudreScenario("C");

    return 0;
}