#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long largeur = 0, hauteur = 0, facteur = 0;
    if (!(std::cin >> largeur >> hauteur >> facteur)) {
        return 0;
    }

    long long vx = 0, vy = 0, vl = 0, vh = 0;
    std::cin >> vx >> vy >> vl >> vh;

    // Dimensions physiques de la fenêtre
    long long W = (largeur * facteur) / 100LL;
    long long H = (hauteur * facteur) / 100LL;

    int n = 0;
    if (!(std::cin >> n)) {
        n = 0;
    }

    int coupes = 0;
    int hors = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long x = 0, y = 0;
        std::cin >> nom >> x >> y;

        if (x < -1000 || x > 1000 || y < -1000 || y > 1000) {
            std::cout << nom << " COUPE\n";
            coupes++;
        } else {
            long long px = vx + (x + 1000LL) * vl / 2000LL;
            long long py = vy + (y + 1000LL) * vh / 2000LL;

            if (px < 0 || px >= W || py < 0 || py >= H) {
                std::cout << nom << " " << px << " " << py << " HORS FENETRE\n";
                hors++;
            } else {
                long long ligne = H - 1LL - py;
                std::cout << nom << " " << px << " " << py << " " << ligne << "\n";
            }
        }
    }

    // Calcul de la couverture
    long long left = std::max(0LL, vx);
    long long right = std::min(W, vx + vl);
    long long width_covered = std::max(0LL, right - left);

    long long bottom = std::max(0LL, vy);
    long long top = std::min(H, vy + vh);
    long long height_covered = std::max(0LL, top - bottom);

    long long couverture = 0;
    if (W * H > 0) {
        couverture = (width_covered * height_covered * 1000LL) / (W * H);
    }

    std::cout << "COUPES " << coupes << "\n";
    std::cout << "HORS " << hors << "\n";
    std::cout << "COUVERTURE " << couverture << "\n";

    return 0;
}