#include <iostream>
#include <string>
#include <vector>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long cout_change = 0, cout_meme = 0, budget_us = 0;
    if (!(std::cin >> cout_change >> cout_meme >> budget_us)) {
        return 0;
    }

    int n = 0;
    if (!(std::cin >> n)) {
        n = 0;
    }

    long long changements = 0;
    long long redondants = 0;
    long long naif = 0;
    long long prudent = 0;
    long long regroupe = 0;

    std::set<std::string> contextes_distincts;
    std::string dernier_contexte = "";

    for (int i = 0; i < n; ++i) {
        std::string ctx;
        std::cin >> ctx;

        contextes_distincts.insert(ctx);

        if (i == 0) {
            changements++;
        } else {
            if (ctx != dernier_contexte) {
                changements++;
            } else {
                redondants++;
            }
        }
        dernier_contexte = ctx;
    }

    if (n > 0) {
        naif = changements * cout_change + redondants * cout_meme;
        prudent = changements * cout_change;
        regroupe = static_cast<long long>(contextes_distincts.size()) * cout_change;
    }

    // Conversion du budget de microsecondes en nanosecondes
    long long budget_ns = budget_us * 1000LL;

    // PART : naif * 1000 / budget_ns (division entière)
    long long part = (naif * 1000LL) / budget_ns;

    // SEUIL : plus petit entier k tel que k * cout_change * 100 >= budget_ns
    // Equivalent à k = ceil(budget_ns / (cout_change * 100))
    long long num = budget_ns;
    long long den = cout_change * 100LL;
    long long seuil = (num + den - 1LL) / den;

    std::cout << "CHANGEMENTS " << changements << "\n";
    std::cout << "REDONDANTS " << redondants << "\n";
    std::cout << "NAIF " << naif << "\n";
    std::cout << "PRUDENT " << prudent << "\n";
    std::cout << "REGROUPE " << regroupe << "\n";
    std::cout << "PART " << part << "\n";
    std::cout << "SEUIL " << seuil << "\n";

    return 0;
}