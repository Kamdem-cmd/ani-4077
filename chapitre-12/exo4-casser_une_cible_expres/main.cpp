#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct Cible {
    std::string nom;
    std::string cf; int cw, ch;
    std::string pf; int pw, ph;
    std::string minFilter;
    std::string magFilter;
    int niv;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string version;
    int N = 0;
    if (!(std::cin >> version >> N)) return 0;

    std::vector<Cible> cibles(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> cibles[i].nom >> cibles[i].cf >> cibles[i].cw >> cibles[i].ch
                 >> cibles[i].pf >> cibles[i].pw >> cibles[i].ph
                 >> cibles[i].minFilter >> cibles[i].magFilter >> cibles[i].niv;
    }

    int completes = 0;
    int noires = 0;
    int refusees = 0;

    for (const auto& c : cibles) {
        // 1. Sans aucune surface
        if (c.cf == "AUCUNE" && c.pf == "AUCUNE") {
            std::cout << c.nom << " MISSING_ATTACHMENT\n";
            refusees++;
            continue;
        }

        bool invalid = false;
        // Vérification couleur
        if (c.cf != "AUCUNE") {
            if (c.cw <= 0 || c.ch <= 0 || c.cf != "RGBA8") invalid = true;
        }
        // Vérification profondeur
        if (c.pf != "AUCUNE") {
            if (c.pw <= 0 || c.ph <= 0) invalid = true;
            if (c.pf != "DEPTH16" && c.pf != "DEPTH24") invalid = true;
            if (version == "ES2" && c.pf == "DEPTH24") invalid = true; // DEPTH24 n'existe pas en ES2
        }

        if (invalid) {
            std::cout << c.nom << " INCOMPLETE_ATTACHMENT\n";
            refusees++;
            continue;
        }

        // Dimensions
        if (c.cf != "AUCUNE" && c.pf != "AUCUNE" && (c.cw != c.pw || c.ch != c.ph)) {
            if (version == "ES2") {
                std::cout << c.nom << " INCOMPLETE_DIMENSIONS\n";
                refusees++;
                continue;
            }
        }

        int targetW = (c.cf != "AUCUNE") ? c.cw : c.pw;
        int targetH = (c.cf != "AUCUNE") ? c.ch : c.ph;
        if (version != "ES2" && c.cf != "AUCUNE" && c.pf != "AUCUNE") {
            targetW = std::min(c.cw, c.pw);
            targetH = std::min(c.ch, c.ph);
        }

        completes++;
        std::cout << c.nom << " COMPLETE " << targetW << "x" << targetH;

        // Lecture texture couleur (mipmaps)
        bool lectureNoire = false;
        if (c.cf != "AUCUNE") {
            std::string minF = c.minFilter;
            if (minF.find("MIPMAP") != std::string::npos) {
                int maxSide = std::max(c.cw, c.ch);
                int reqLevels = 1 + static_cast<int>(std::floor(std::log2(maxSide)));
                if (c.niv < reqLevels) {
                    lectureNoire = true;
                }
            }
        }

        if (lectureNoire) {
            std::cout << " LECTURE NOIRE\n";
            noires++;
        } else {
            std::cout << " LECTURE OK\n";
        }
    }

    std::cout << "COMPLETES " << completes << "\n";
    std::cout << "NOIRES " << noires << "\n";
    std::cout << "REFUSEES " << refusees << "\n";

    return 0;
}