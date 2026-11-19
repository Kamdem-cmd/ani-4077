#include <iostream>
#include <string>
#include <vector>
#include <stack>

struct Case {
    std::string nom;
    uint64_t generation = 1;
    bool occupee = false;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::vector<Case> cases;
    std::stack<uint64_t> libres;
    long long retraits = 0;

    for (int n = 0; n < N; ++n) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "POSER") {
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
            std::cout << "POSER " << nom << " " << idx << " " << cases[idx].generation << "\n";
        } else if (cmd == "RETIRER") {
            uint64_t i, g;
            std::cin >> i >> g;
            if (g != 0 && i < cases.size() && cases[i].occupee && cases[i].generation == g) {
                std::cout << "RETIRER " << i << " " << g << " " << cases[i].nom << "\n";
                cases[i].occupee = false;
                cases[i].nom.clear();
                cases[i].generation++;
                libres.push(i);
                retraits++;
            } else {
                std::cout << "RETIRER " << i << " " << g << " RIEN\n";
            }
        } else if (cmd == "TROUVER") {
            uint64_t i, g;
            std::cin >> i >> g;
            if (g == 0) {
                std::cout << "TROUVER " << i << " " << g << " NULLE\n";
            } else if (i >= cases.size()) {
                std::cout << "TROUVER " << i << " " << g << " HORS_BORNES\n";
            } else if (!cases[i].occupee) {
                std::cout << "TROUVER " << i << " " << g << " VIDE\n";
            } else if (cases[i].generation != g) {
                std::cout << "TROUVER " << i << " " << g << " PERIMEE\n";
            } else {
                std::cout << "TROUVER " << i << " " << g << " " << cases[i].nom << "\n";
            }
        }
    }

    size_t vivantes = 0;
    for (const auto& c : cases) {
        if (c.occupee) vivantes++;
    }

    std::cout << "VIVANTS " << vivantes << "\n";
    std::cout << "CASES " << cases.size() << "\n";
    std::cout << "RETRAITS " << retraits << "\n";

    return 0;
}