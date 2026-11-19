#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cstdint>

struct MarqueMaillage {};
struct MarqueTexture {};

template <typename Marque>
struct Poignee {
    uint64_t index = 0;
    uint64_t generation = 0;
};

using PoigneeMaillage = Poignee<MarqueMaillage>;
using PoigneeTexture = Poignee<MarqueTexture>;

class IRhi {
public:
    virtual ~IRhi() = default;

    virtual PoigneeMaillage creerMaillage() = 0;
    virtual void detruireMaillage(PoigneeMaillage p) = 0;
    virtual bool estValideMaillage(PoigneeMaillage p) const = 0;

    virtual PoigneeTexture creerTexture() = 0;
    virtual void detruireTexture(PoigneeTexture p) = 0;
    virtual bool estValideTexture(PoigneeTexture p) const = 0;
};

class RhiMuet : public IRhi {
private:
    struct Case {
        uint64_t generation = 1;
        bool occupee = false;
    };

    template <typename Marque>
    struct ReserveTypee {
        std::vector<Case> cases;
        std::stack<uint64_t> libres;

        Poignee<Marque> creer() {
            uint64_t idx;
            if (!libres.empty()) {
                idx = libres.top();
                libres.pop();
            } else {
                idx = cases.size();
                cases.push_back({1, false});
            }
            cases[idx].occupee = true;
            return {idx, cases[idx].generation};
        }

        bool estValide(Poignee<Marque> p) const {
            if (p.generation == 0) return false;
            if (p.index >= cases.size()) return false;
            if (!cases[p.index].occupee) return false;
            return cases[p.index].generation == p.generation;
        }

        void detruire(Poignee<Marque> p) {
            if (!estValide(p)) return;
            cases[p.index].occupee = false;
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

        size_t totalCases() const {
            return cases.size();
        }
    };

    ReserveTypee<MarqueMaillage> maillages_;
    ReserveTypee<MarqueTexture> textures_;

public:
    PoigneeMaillage creerMaillage() override {
        return maillages_.creer();
    }

    void detruireMaillage(PoigneeMaillage p) override {
        maillages_.detruire(p);
    }

    bool estValideMaillage(PoigneeMaillage p) const override {
        return maillages_.estValide(p);
    }

    PoigneeTexture creerTexture() override {
        return textures_.creer();
    }

    void detruireTexture(PoigneeTexture p) override {
        textures_.detruire(p);
    }

    bool estValideTexture(PoigneeTexture p) const override {
        return textures_.estValide(p);
    }

    size_t texturesVivantes() const {
        return textures_.vivantes();
    }

    size_t texturesCases() const {
        return textures_.totalCases();
    }
};

int executerBanc() {
    int reussies = 0;

    // 1. poignee nulle refusee
    {
        RhiMuet rhi;
        PoigneeTexture nulle{0, 0};
        bool ok = !rhi.estValideTexture(nulle);
        rhi.detruireTexture(nulle);
        ok = ok && (rhi.texturesCases() == 0);
        std::cout << "BANC poignee nulle refusee " << (ok ? "OK" : "ECHEC") << "\n";
        if (ok) reussies++;
    }

    // 2. double destruction sans effet
    {
        RhiMuet rhi;
        PoigneeTexture t1 = rhi.creerTexture();
        PoigneeTexture t2 = rhi.creerTexture();
        rhi.detruireTexture(t1);
        rhi.detruireTexture(t1); // seconde destruction sans effet
        bool ok = !rhi.estValideTexture(t1) && rhi.estValideTexture(t2) && (rhi.texturesVivantes() == 1);
        std::cout << "BANC double destruction sans effet " << (ok ? "OK" : "ECHEC") << "\n";
        if (ok) reussies++;
    }

    // 3. case reemployee
    {
        RhiMuet rhi;
        PoigneeTexture t1 = rhi.creerTexture();
        rhi.detruireTexture(t1);
        PoigneeTexture t2 = rhi.creerTexture();
        bool ok = (t2.index == t1.index) && (rhi.texturesCases() == 1);
        std::cout << "BANC case reemployee " << (ok ? "OK" : "ECHEC") << "\n";
        if (ok) reussies++;
    }

    // 4. generation avancee
    {
        RhiMuet rhi;
        PoigneeTexture t1 = rhi.creerTexture();
        rhi.detruireTexture(t1);
        PoigneeTexture t2 = rhi.creerTexture();
        bool ok = (t2.generation == t1.generation + 1);
        std::cout << "BANC generation avancee " << (ok ? "OK" : "ECHEC") << "\n";
        if (ok) reussies++;
    }

    // 5. perimee invalide apres recyclage
    {
        RhiMuet rhi;
        PoigneeTexture t1 = rhi.creerTexture();
        rhi.detruireTexture(t1);
        PoigneeTexture t2 = rhi.creerTexture();
        bool ok = !rhi.estValideTexture(t1) && rhi.estValideTexture(t2);
        std::cout << "BANC perimee invalide apres recyclage " << (ok ? "OK" : "ECHEC") << "\n";
        if (ok) reussies++;
    }

    // 6. types separes
    {
        RhiMuet rhi;
        PoigneeMaillage m1 = rhi.creerMaillage();
        PoigneeTexture t1 = rhi.creerTexture();
        rhi.detruireMaillage(m1);
        bool ok = !rhi.estValideMaillage(m1) && rhi.estValideTexture(t1);
        std::cout << "BANC types separes " << (ok ? "OK" : "ECHEC") << "\n";
        if (ok) reussies++;
    }

    std::cout << "BANC " << reussies << " SUR 6\n";
    return reussies;
}

void resoudreScenario(const std::string& labelScen) {
    size_t N = 0, M = 0;
    if (!(std::cin >> N >> M)) return;

    RhiMuet rhi;
    std::vector<PoigneeTexture> anciennes;
    anciennes.reserve(N);

    for (size_t i = 0; i < N; ++i) {
        anciennes.push_back(rhi.creerTexture());
    }
    for (size_t i = 0; i < N; ++i) {
        rhi.detruireTexture(anciennes[i]);
    }

    std::vector<PoigneeTexture> nouvelles;
    nouvelles.reserve(M);
    for (size_t i = 0; i < M; ++i) {
        nouvelles.push_back(rhi.creerTexture());
    }

    size_t anciennesValides = 0;
    for (const auto& p : anciennes) {
        if (rhi.estValideTexture(p)) {
            anciennesValides++;
        }
    }

    std::cout << "SCENARIO " << labelScen << "\n";
    if (M == 0) {
        std::cout << "PREMIERE AUCUNE\n";
        std::cout << "DERNIERE AUCUNE\n";
    } else {
        std::cout << "PREMIERE " << nouvelles.front().index << " " << nouvelles.front().generation << "\n";
        std::cout << "DERNIERE " << nouvelles.back().index << " " << nouvelles.back().generation << "\n";
    }
    std::cout << "ANCIENNES VALIDES " << anciennesValides << "\n";
    std::cout << "VIVANTES " << rhi.texturesVivantes() << "\n";
    std::cout << "CASES " << rhi.texturesCases() << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    executerBanc();
    resoudreScenario("A");
    resoudreScenario("B");
    resoudreScenario("C");

    return 0;
}