#include <iostream>
#include <string>
#include <vector>

struct Segment {
    char c;
    int debut, longueur, z;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int W = 0, S = 0, F = 0;
    if (!(std::cin >> W >> S >> F)) return 0;

    std::vector<Segment> segs(S);
    for (int i = 0; i < S; ++i) {
        std::cin >> segs[i].c >> segs[i].debut >> segs[i].longueur >> segs[i].z;
    }

    std::string colorBuf(W, '.');
    std::vector<int> depthBuf(W, 100);

    int vides = 0;

    for (int img = 1; img <= F; ++img) {
        std::string op;
        std::cin >> op;
        if (op == "REDIM") {
            colorBuf = std::string(W, '.');
            depthBuf = std::assign(W, 100); // Réinitialisation complète
            std::cin >> op; // Lire l'opération d'effacement suivante
        }

        if (op == "TOUT") {
            colorBuf = std::string(W, '.');
            std::fill(depthBuf.begin(), depthBuf.end(), 100);
        } else if (op == "COULEUR") {
            colorBuf = std::string(W, '.');
        } else if (op == "PROFONDEUR") {
            std::fill(depthBuf.begin(), depthBuf.end(), 100);
        } else if (op == "RIEN") {
            // Ne touche à rien
        }

        int passes = 0;
        for (const auto& seg : segs) {
            for (int i = 0; i < seg.longueur; ++i) {
                int pos = seg.debut + i;
                if (pos >= 0 && pos < W) {
                    if (seg.z < depthBuf[pos]) {
                        colorBuf[pos] = seg.c;
                        depthBuf[pos] = seg.z;
                        passes++;
                    }
                }
            }
        }

        if (passes == 0) {
            vides++;
        }

        std::cout << "IMAGE " << img << " " << colorBuf << " PASSES " << passes << "\n";
    }

    std::cout << "VIDES " << vides << "\n";

    return 0;
}