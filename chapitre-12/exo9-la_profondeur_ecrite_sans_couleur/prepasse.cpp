#include <iostream>
#include <string>
#include <vector>

struct Rect {
    char c;
    int x, y, l, h, z;
};

void resoudreScenario(const std::string& labelScen) {
    int W = 0, H = 0, N = 0;
    long long F = 0, P = 0, S = 0;
    if (!(std::cin >> W >> H >> N >> F >> P >> S)) return;

    std::vector<Rect> rects(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> rects[i].c >> rects[i].x >> rects[i].y >> rects[i].l >> rects[i].h >> rects[i].z;
    }

    // 1. Sans pré-passe (GL_LESS)
    std::vector<std::vector<int>> depth1(H, std::vector<int>(W, 100));
    long long sansNuanceur = 0;
    for (const auto& r : rects) {
        for (int ry = r.y; ry < r.y + r.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = r.x; rx < r.x + r.l; ++rx) {
                if (rx < 0 || rx >= W) continue;
                if (r.z < depth1[ry][rx]) {
                    depth1[ry][rx] = r.z;
                    sansNuanceur++;
                }
            }
        }
    }
    long long coutSans = N * S + sansNuanceur * F;

    // 2. Avec pré-passe
    // Passe 1 (GL_LESS) : écriture profondeur seule
    std::vector<std::vector<int>> depth2(H, std::vector<int>(W, 100));
    long long avecProfondeur = 0;
    for (const auto& r : rects) {
        for (int ry = r.y; ry < r.y + r.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = r.x; rx < r.x + r.l; ++rx) {
                if (rx < 0 || rx >= W) continue;
                if (r.z < depth2[ry][rx]) {
                    depth2[ry][rx] = r.z;
                    avecProfondeur++;
                }
            }
        }
    }

    // Passe 2 (GL_LEQUAL) : execution nuanceur coûteux
    long long avecNuanceur = 0;
    for (const auto& r : rects) {
        for (int ry = r.y; ry < r.y + r.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = r.x; rx < r.x + r.l; ++rx) {
                if (rx < 0 || rx >= W) continue;
                if (r.z <= depth2[ry][rx]) {
                    avecNuanceur++;
                }
            }
        }
    }
    long long coutAvec = 2 * N * S + avecProfondeur * P + avecNuanceur * F;

    // Test Passe 2 en GL_LESS
    long long passe2Less = 0;
    for (const auto& r : rects) {
        for (int ry = r.y; ry < r.y + r.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = r.x; rx < r.x + r.l; ++rx) {
                if (rx < 0 || rx >= W) continue;
                if (r.z < depth2[ry][rx]) {
                    passe2Less++;
                }
            }
        }
    }

    std::cout << "SCENARIO " << labelScen << "\n";
    std::cout << "SANS NUANCEUR " << sansNuanceur << " COUT " << coutSans << "\n";
    std::cout << "AVEC PROFONDEUR " << avecProfondeur << " NUANCEUR " << avecNuanceur << " COUT " << coutAvec << "\n";
    std::cout << "PASSE 2 EN LESS NUANCEUR " << passe2Less << "\n";
    std::cout << "GAGNE " << (coutSans - coutAvec) << "\n";
    if (coutAvec < coutSans) {
        std::cout << "VERDICT PREPASSE\n";
    } else {
        std::cout << "VERDICT UNE PASSE\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");
    resoudreScenario("C");

    return 0;
}