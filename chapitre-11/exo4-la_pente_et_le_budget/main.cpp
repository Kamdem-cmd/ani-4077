#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

struct ResultatScene {
    long long creationLiaisons;
    long long regroupeLiaisons;
    long long utiles;
};

ResultatScene calculerScene(long long n, long long S, long long T, long long G, int D) {
    if (n == 0) return {0, 0, 0};

    // Nombre total de liaisons en ordre de création :
    // Chaque objet lie P, lie T, lie M=1, et (si D==1) lie M=0
    long long parObjetCreation = (D == 1) ? 4 : 3;
    long long a = n * parObjetCreation;

    // Déterminer les programmes et couples (P, T) distincts utilisés
    // P_i = (i / G) % S + 1
    // T_i = i % T + 1
    // Comme P est périodique de période G * S, et T de période T,
    // le couple (P_i, T_i) est périodique de période L_p = lcm(G * S, T).
    // Pour n grand, on peut borner l'exploration à min(n, P_period).
    long long periodP = G * S;
    // Calcul du PPCM de periodP et T
    long long gcdVal = 1;
    {
        long long x = periodP, y = T;
        while (y != 0) {
            long long tmp = y;
            y = x % y;
            x = tmp;
        }
        gcdVal = x;
    }
    long long lcmPeriod = (periodP / gcdVal) * T;
    long long limit = std::min(n, lcmPeriod);

    std::set<long long> usedProgs;
    std::set<std::pair<long long, long long>> usedPairs;

    for (long long i = 0; i < limit; ++i) {
        long long p = (i / G) % S + 1;
        long long t = i % T + 1;
        usedProgs.insert(p);
        usedPairs.insert({p, t});
    }

    long long nbProgs = static_cast<long long>(usedProgs.size());
    long long nbPairs = static_cast<long long>(usedPairs.size());

    // Liaisons en ordre regroupé :
    // nbProgs (liaisons P) + nbPairs (liaisons T) + n (liaisons M=1) + (D == 1 ? n : 0)
    long long c = nbProgs + nbPairs + n * ((D == 1) ? 2 : 1);

    // Liaisons utiles en ordre regroupé :
    // Chaque programme utilisé est distinct -> nbProgs utiles
    // Chaque paire (p, t) liée sous son p : le premier t d'un p vaut toujours > 0 (car t >= 1 et lastT commence à 0 ou au dernier t du p précédent).
    // Attention : si le dernier t du programme précédent est égal au premier t du programme suivant, cette liaison T est inutile !
    std::map<long long, std::set<long long>> progToTex;
    for (const auto& pr : usedPairs) {
        progToTex[pr.first].insert(pr.second);
    }

    long long utilesT = 0;
    long long lastT = 0;
    for (const auto& [p, tSet] : progToTex) {
        for (long long t : tSet) {
            if (t != lastT) {
                utilesT++;
                lastT = t;
            }
        }
    }

    long long utilesM = (D == 1) ? (2 * n) : 1;
    long long u = nbProgs + utilesT + utilesM;

    return {a, c, u};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long S, T, G, B;
    int D, Q;
    if (!(std::cin >> S >> T >> G >> D >> B >> Q)) return 0;

    for (int q = 0; q < Q; ++q) {
        long long n;
        std::cin >> n;
        ResultatScene res = calculerScene(n, S, T, G, D);
        std::cout << "OBJETS " << n 
                  << " CREATION " << res.creationLiaisons 
                  << " REGROUPE " << res.regroupeLiaisons 
                  << " UTILES " << res.utiles << "\n";
    }

    // Recherche binaire pour MAX CREATION
    long long low = 0, high = B + 1, maxCreation = 0;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (calculerScene(mid, S, T, G, D).creationLiaisons <= B) {
            maxCreation = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Recherche binaire pour MAX REGROUPE
    low = 0; high = B + 1;
    long long maxRegroupe = 0;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (calculerScene(mid, S, T, G, D).regroupeLiaisons <= B) {
            maxRegroupe = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    std::cout << "MAX CREATION " << maxCreation << "\n";
    std::cout << "MAX REGROUPE " << maxRegroupe << "\n";

    return 0;
}