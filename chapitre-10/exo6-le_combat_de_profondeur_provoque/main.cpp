#include <iostream>
#include <cmath>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int b = 0, N = 0;
    if (!(std::cin >> b >> N)) return 0;

    long long M = (1LL << b) - 1;
    long long sepSure = (2000000 + M - 1) / M; // Arrondi vers le haut

    int erreurs = 0;

    for (int i = 1; i <= N; ++i) {
        long long z1, z2;
        std::cin >> z1 >> z2;

        // Formule : d = (2 * (z + 1000000) * M + 2000000) / 4000000
        long long d1 = (2 * (z1 + 1000000) * M + 2000000) / 4000000;
        long long d2 = (2 * (z2 + 1000000) * M + 2000000) / 4000000;

        int gagnant = 1;
        if (d2 < d1) {
            gagnant = 2;
        }

        // Le verdict vaut OK si le gagnant est la surface au plus petit z (ou égalité)
        std::string verdict = "OK";
        bool expectedGagnant1 = (z1 <= z2);
        if ((gagnant == 1 && !expectedGagnant1) || (gagnant == 2 && expectedGagnant1)) {
            verdict = "ERREUR";
            erreurs++;
        }

        std::cout << "PAIRE " << i << " " << d1 << " " << d2 << " GAGNANT " << gagnant << " " << verdict << "\n";
    }

    std::cout << "ERREURS " << erreurs << "\n";
    std::cout << "SEPARATION SURE " << sepSure << "\n";

    return 0;
}