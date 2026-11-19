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

    bool detruire(Poignee p) {
        if (!estBonne(p)) return false;
        cases[p.index].occupee = false;
        cases[p.index].nom.clear();
        cases[p.index].generation++;
        libres.push(p.index);
        return true;
    }

    size_t vivantes() const {
        size_t count = 0;
        for (const auto& c : cases) {
            if (c.occupee) count++;
        }
        return count;
    }
};

struct Instruction {
    int ligne;
    std::string op;   // CREER, DETRUIRE, VOIR
    std::string var;
    std::string type;
    std::string nom;
    bool refusee = false;
};

struct Refus {
    int ligne;
    std::string typeV;
    std::string typeAttendu;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::vector<Instruction> script;
    script.reserve(N);

    std::map<std::string, std::string> varTypes;
    std::vector<Refus> refusList;

    for (int i = 1; i <= N; ++i) {
        std::string premier;
        std::cin >> premier;
        Instruction inst;
        inst.ligne = i;

        if (premier == "DETRUIRE" || premier == "VOIR") {
            inst.op = premier;
            std::cin >> inst.type >> inst.var;
            std::string typeV = varTypes[inst.var];
            if (typeV != inst.type) {
                inst.refusee = true;
                refusList.push_back({i, typeV, inst.type});
            }
        } else {
            // v = CREER TYPE nom
            inst.var = premier;
            std::string eq;
            std::cin >> eq >> inst.op >> inst.type >> inst.nom;
            varTypes[inst.var] = inst.type;
        }
        script.push_back(inst);
    }

    if (refusList.empty()) {
        std::cout << "COMPILATION ACCEPTEE\n";
    } else {
        std::cout << "COMPILATION REFUSEE\n";
        for (const auto& r : refusList) {
            std::cout << "REFUS LIGNE " << r.ligne << " " << r.typeV << " VERS " << r.typeAttendu << "\n";
        }
    }

    std::cout << "EXECUTION SANS MARQUE\n";

    std::map<std::string, Reserve> reserves;
    std::map<std::string, Poignee> varPoignees;

    long long detruitsATort = 0;
    long long vusATort = 0;

    for (const auto& inst : script) {
        if (inst.op == "CREER") {
            varPoignees[inst.var] = reserves[inst.type].creer(inst.nom);
        } else if (inst.op == "DETRUIRE") {
            bool detruit = reserves[inst.type].detruire(varPoignees[inst.var]);
            if (inst.refusee && detruit) {
                detruitsATort++;
            }
        } else if (inst.op == "VOIR") {
            Poignee p = varPoignees[inst.var];
            if (reserves[inst.type].estBonne(p)) {
                std::cout << "VOIR LIGNE " << inst.ligne << " " << reserves[inst.type].cases[p.index].nom << "\n";
                if (inst.refusee) {
                    vusATort++;
                }
            } else {
                std::cout << "VOIR LIGNE " << inst.ligne << " PERIMEE\n";
            }
        }
    }

    std::cout << "DETRUITS A TORT " << detruitsATort << "\n";
    std::cout << "VUS A TORT " << vusATort << "\n";
    std::cout << "VIVANTS " << reserves["MAILLAGE"].vivantes() << " "
              << reserves["NUANCEUR"].vivantes() << " "
              << reserves["TEXTURE"].vivantes() << " "
              << reserves["CIBLE"].vivantes() << "\n";

    return 0;
}