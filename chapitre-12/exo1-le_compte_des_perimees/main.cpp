#include <iostream>
#include <string>
#include <vector>
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
    std::vector<Case> cases;
    std::stack<size_t> libres;

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
        return cases[p.index].generation == p.generation;
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

    int T = 0, O = 0;
    if (!(std::cin >> T >> O)) return 0;

    Reserve reserve;
    std::vector<Poignee> textures(1); // indexé à partir de 1

    for (int i = 0; i < T; ++i) {
        std::string nom;
        std::cin >> nom;
        textures.push_back(reserve.creer(nom));
    }

    std::vector<int> objets(O);
    for (int i = 0; i < O; ++i) {
        std::cin >> objets[i];
    }

    int C = 0;
    std::cin >> C;

    long long totalPerimees = 0;
    long long imageNum = 0;

    for (int c = 0; c < C; ++c) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "DETRUIRE") {
            size_t k;
            std::cin >> k;
            if (k >= 1 && k < textures.size()) {
                reserve.detruire(textures[k]);
            }
        } else if (cmd == "CREER") {
            std::string nom;
            std::cin >> nom;
            textures.push_back(reserve.creer(nom));
        } else if (cmd == "VALIDE") {
            size_t k;
            std::cin >> k;
            bool ok = (k >= 1 && k < textures.size()) && reserve.estBonne(textures[k]);
            std::cout << "VALIDE " << k << " " << (ok ? "OUI" : "NON") << "\n";
        } else if (cmd == "IMAGE") {
            imageNum++;
            long long perimeesImage = 0;
            std::vector<std::string> vues;
            vues.reserve(O);

            for (int objTexId : objets) {
                if (objTexId >= 1 && static_cast<size_t>(objTexId) < textures.size() &&
                    reserve.estBonne(textures[objTexId])) {
                    vues.push_back(reserve.cases[textures[objTexId].index].nom);
                } else {
                    vues.push_back("MANQUANTE");
                    perimeesImage++;
                }
            }

            totalPerimees += perimeesImage;
            std::cout << "IMAGE " << imageNum << " PERIMEES " << perimeesImage << " VUES";
            if (O == 0) {
                std::cout << " AUCUN";
            } else {
                for (const auto& v : vues) {
                    std::cout << " " << v;
                }
            }
            std::cout << "\n";
        }
    }

    std::cout << "TOTAL PERIMEES " << totalPerimees << "\n";
    std::cout << "VIVANTES " << reserve.vivantes() << "\n";

    return 0;
}