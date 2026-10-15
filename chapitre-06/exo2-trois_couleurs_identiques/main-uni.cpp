#include <iostream>
#include <string>
#include <vector>
#include <numeric>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int E = 0;
    if (!(std::cin >> E)) {
        return 0;
    }

    const std::vector<std::string> noms_temps = {
        "peripherique", "format", "contexte", "courant", "chargement"
    };

    std::vector<long long> sommes(5, 0);

    for (int i = 0; i < E; ++i) {
        for (int j = 0; j < 5; ++j) {
            long long val = 0;
            std::cin >> val;
            sommes[j] += val;
        }
    }

    long long somme_totale = 0;
    for (int j = 0; j < 5; ++j) {
        somme_totale += sommes[j];
    }

    int idx_plus_cher = 0;
    long long max_somme = -1;

    for (int j = 0; j < 5; ++j) {
        long long moy = (E > 0) ? (sommes[j] / E) : 0;
        long long part = (somme_totale > 0) ? (sommes[j] * 1000LL / somme_totale) : 0;

        if (sommes[j] > max_somme) {
            max_somme = sommes[j];
            idx_plus_cher = j;
        }

        std::cout << noms_temps[j] << " " << moy << " " << part << "\n";
    }

    long long total = (E > 0) ? (somme_totale / E) : 0;
    long long images = (total + 16666LL) / 16667LL;

    std::cout << "PLUS CHER " << noms_temps[idx_plus_cher] << "\n";
    std::cout << "TOTAL " << total << "\n";
    std::cout << "IMAGES " << images << "\n";

    return 0;
}