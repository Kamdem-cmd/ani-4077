#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    long long trompeurs = 0;
    std::string meilleurNom = "AUCUN";
    long long minL = -1;

    long long L0 = 0, I0 = 0;

    for (int idx = 0; idx < N; ++idx) {
        std::string nom;
        long long L, I;
        std::cin >> nom >> L >> I;

        long long p = I * 100 / L;
        long long u = L - I;

        if (minL == -1 || L < minL) {
            minL = L;
            meilleurNom = nom;
        }

        if (idx == 0) {
            std::cout << nom << " PART " << p << " % UTILES " << u << " DEPART\n";
        } else {
            std::string totalDir = "STABLE";
            if (L > L0) totalDir = "MONTE";
            else if (L < L0) totalDir = "BAISSE";

            std::string partDir = "STABLE";
            long long lhs = I * L0;
            long long rhs = I0 * L;
            if (lhs > rhs) partDir = "MONTE";
            else if (lhs < rhs) partDir = "BAISSE";

            std::string verdict = "COHERENT";
            if ((totalDir == "MONTE" && partDir == "BAISSE") ||
                (totalDir == "BAISSE" && partDir == "MONTE")) {
                verdict = "TROMPEUR";
                trompeurs++;
            }

            std::cout << nom << " PART " << p << " % UTILES " << u 
                      << " TOTAL " << totalDir << " PART " << partDir 
                      << " " << verdict << "\n";
        }

        L0 = L;
        I0 = I;
    }

    std::cout << "TROMPEURS " << trompeurs << "\n";
    std::cout << "MEILLEUR " << meilleurNom << "\n";

    return 0;
}