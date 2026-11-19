#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct CibleInfo {
    int id;
    long long w, h;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long winW = 0, winH = 0;
    if (!(std::cin >> winW >> winH)) return 0;

    int N = 0;
    std::cin >> N;

    std::vector<CibleInfo> cibles;
    cibles.push_back({0, winW, winH}); // Cible 0 = fenêtre

    int currentCible = 0;
    long long vpX = 0, vpY = 0, vpL = winW, vpH = winH;

    int dessinCount = 0;
    int malCadres = 0;
    int pointCount = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "CIBLE") {
            int id; long long l, h;
            std::cin >> id >> l >> h;
            cibles.push_back({id, l, h});
        } else if (cmd == "LIER") {
            std::cin >> currentCible;
        } else if (cmd == "VIEWPORT") {
            std::cin >> vpX >> vpY >> vpL >> vpH;
        } else if (cmd == "DESSIN") {
            dessinCount++;
            long long L = 0, H = 0;
            for (const auto& c : cibles) {
                if (c.id == currentCible) {
                    L = c.w; H = c.h;
                    break;
                }
            }

            long long x0 = std::max(vpX, 0LL);
            long long y0 = std::max(vpY, 0LL);
            long long x1 = std::min(vpX + vpL, L);
            long long y1 = std::min(vpY + vpH, H);

            if (x0 >= x1 || y0 >= y1) {
                std::cout << "DESSIN " << dessinCount << " CIBLE " << currentCible << " DEHORS\n";
                malCadres++;
            } else {
                long long aire = (x1 - x0) * (y1 - y0);
                long long scene = aire * 100 / (vpL * vpH);
                long long couvre = aire * 100 / (L * H);

                std::string verdict = "JUSTE";
                if (vpX != 0 || vpY != 0 || vpL != L || vpH != H) {
                    if (vpX >= 0 && vpY >= 0 && vpX + vpL <= L && vpY + vpH <= H) {
                        verdict = "COIN";
                    } else {
                        verdict = "ROGNE";
                    }
                    malCadres++;
                }

                std::cout << "DESSIN " << dessinCount << " CIBLE " << currentCible 
                          << " ZONE " << x0 << " " << y0 << " " << x1 << " " << y1 
                          << " SCENE " << scene << " COUVRE " << couvre << " VERDICT " << verdict << "\n";
            }
        } else if (cmd == "POINT") {
            pointCount++;
            long long nx, ny;
            std::cin >> nx >> ny;

            long long L = 0, H = 0;
            for (const auto& c : cibles) {
                if (c.id == currentCible) {
                    L = c.w; H = c.h;
                    break;
                }
            }

            long long px = vpX + (nx + 100) * vpL / 200;
            long long py = vpY + (ny + 100) * vpH / 200;

            std::cout << "POINT " << pointCount << " " << px << " " << py;
            if (px < 0 || px >= L || py < 0 || py >= H) {
                std::cout << " HORS";
            }
            std::cout << "\n";
        }
    }

    std::cout << "DESSINS " << dessinCount << "\n";
    std::cout << "MAL CADRES " << malCadres << "\n";

    return 0;
}