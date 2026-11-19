#include <iostream>
#include <cstdint>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int Q = 0;
    if (!(std::cin >> Q)) return 0;

    if (Q == 0) {
        std::cout << "AUCUN\n";
        return 0;
    }

    for (int q = 1; q <= Q; ++q) {
        uint64_t B = 0, K = 0, R = 1;
        std::cin >> B >> K >> R;

        uint64_t mod = 1ULL << B;
        uint64_t gen = (1ULL + (K % mod)) % mod;
        const char* premiere = (gen == 1ULL) ? "VALIDE" : "PERIMEE";

        // Cycles où (1 + c) % mod == 0  <=>  c % mod == mod - 1 (pour 1 <= c <= K)
        uint64_t nulles = (K + 1ULL) / mod;

        // Cycles où (1 + c) % mod == 1  <=>  c % mod == 0 (pour 1 <= c <= K)
        uint64_t resurrections = K / mod;

        // Parade : une case sert aux générations 1 à mod - 1, soit mod - 1 poses.
        // Il y a K + 1 poses au total (l'initiale + K cycles).
        uint64_t maxPosesParCase = mod - 1ULL;
        uint64_t totalPoses = K + 1ULL;
        uint64_t cases = (totalPoses + maxPosesParCase - 1ULL) / maxPosesParCase;

        // Jours entiers avant la première résurrection (mod cycles à R cycles/s)
        uint64_t jours = mod / (R * 86400ULL);

        std::cout << "REQUETE " << q
                  << " GENERATION " << gen
                  << " PREMIERE " << premiere
                  << " NULLES " << nulles
                  << " RESURRECTIONS " << resurrections
                  << " CASES " << cases
                  << " JOURS " << jours << "\n";
    }

    return 0;
}