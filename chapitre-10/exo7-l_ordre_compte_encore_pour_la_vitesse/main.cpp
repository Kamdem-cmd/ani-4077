#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Objet {
    char c;
    int x, y, l, h, z;
    int indexLecture;
};

void simuler(const std::vector<Objet>& objets, int W, int H, long long& nuanceurOut, long long& rejetesOut) {
    std::vector<std::vector<int>> depth(H, std::vector<int>(W, 100));
    nuanceurOut = 0;
    rejetesOut = 0;

    for (const auto& obj : objets) {
        if (obj.z == 100) {
            // Un objet à z = 100 ne passe jamais LESS
            for (int ry = obj.y; ry < obj.y + obj.h; ++ry) {
                if (ry < 0 || ry >= H) continue;
                for (int rx = obj.x; rx < obj.x + obj.l; ++rx) {
                    if (rx < 0 || rx >= W) continue;
                    rejetesOut++;
                }
            }
            continue;
        }

        for (int ry = obj.y; ry < obj.y + obj.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = obj.x; rx < obj.x + obj.l; ++rx) {
                if (rx < 0 || rx >= W) continue;

                if (obj.z < depth[ry][rx]) {
                    nuanceurOut++;
                    depth[ry][rx] = obj.z;
                } else {
                    rejetesOut++;
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int W = 0, H = 0, N = 0;
    if (!(std::cin >> W >> H >> N)) return 0;

    if (N == 0) {
        std::cout << "AUCUN\n";
        return 0;
    }

    std::vector<Objet> donne(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> donne[i].c >> donne[i].x >> donne[i].y >> donne[i].l >> donne[i].h >> donne[i].z;
        donne[i].indexLecture = i;
    }

    // DONNE
    long long nDonne = 0, rDonne = 0;
    simuler(donne, W, H, nDonne, rDonne);

    // PROCHE_DABORD (z croissant, tri stable par index)
    std::vector<Objet> proche = donne;
    std::stable_sort(proche.begin(), proche.end(), [](const Objet& a, const Objet& b) {
        return a.z < b.z;
    });
    long long nProche = 0, rProche = 0;
    simuler(proche, W, H, nProche, rProche);

    // LOIN_DABORD (z décroissant, tri stable par index)
    std::vector<Objet> loin = donne;
    std::stable_sort(loin.begin(), loin.end(), [](const Objet& a, const Objet& b) {
        return a.z > b.z;
    });
    long long nLoin = 0, rLoin = 0;
    simuler(loin, W, H, nLoin, rLoin);

    // Pixels couverts (ordre DONNE)
    std::vector<std::vector<bool>> covered(H, std::vector<bool>(W, false));
    long long couvertsCount = 0;
    for (const auto& obj : donne) {
        if (obj.z == 100) continue;
        for (int ry = obj.y; ry < obj.y + obj.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = obj.x; rx < obj.x + obj.l; ++rx) {
                if (rx < 0 || rx >= W) continue;
                if (!covered[ry][rx]) {
                    covered[ry][rx] = true;
                    couvertsCount++;
                }
            }
        }
    }

    // MEILLEUR
    std::string meilleur = "DONNE";
    long long minN = nDonne;
    if (nProche < minN) {
        minN = nProche;
        meilleur = "PROCHE_DABORD";
    }
    if (nLoin < minN) {
        minN = nLoin;
        meilleur = "LOIN_DABORD";
    }

    long long gaspilles = nDonne - couvertsCount;

    std::cout << "DONNE NUANCEUR " << nDonne << " REJETES " << rDonne << "\n";
    std::cout << "PROCHE_DABORD NUANCEUR " << nProche << " REJETES " << rProche << "\n";
    std::cout << "LOIN_DABORD NUANCEUR " << nLoin << " REJETES " << rLoin << "\n";
    std::cout << "COUVERTS " << couvertsCount << "\n";
    std::cout << "MEILLEUR " << meilleur << "\n";
    std::cout << "GASPILLES " << gaspilles << "\n";

    return 0;
}