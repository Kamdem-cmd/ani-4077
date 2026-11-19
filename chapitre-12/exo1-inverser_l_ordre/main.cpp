#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Rect {
    char c;
    int x, y, l, h, z;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int W = 0, H = 0;
    if (!(std::cin >> W >> H)) return 0;

    int N = 0;
    std::cin >> N;

    std::vector<std::string> grid(H, std::string(W, '.'));
    std::vector<std::vector<int>> depth(H, std::vector<int>(W, 100));

    bool testOn = false;
    std::vector<Rect> rects;
    std::vector<char> orderLetters;
    std::map<char, bool> seenLetter;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "TEST") {
            std::string state;
            std::cin >> state;
            testOn = (state == "ON");
        } else if (cmd == "RECT") {
            char c;
            int x, y, l, h, z;
            std::cin >> c >> x >> y >> l >> h >> z;
            rects.push_back({c, x, y, l, h, z});
            if (!seenLetter[c]) {
                seenLetter[c] = true;
                orderLetters.push_back(c);
            }
        }
    }

    int rejetes = 0;
    std::map<char, int> visibleCount;

    for (const auto& r : rects) {
        for (int ry = r.y; ry < r.y + r.h; ++ry) {
            if (ry < 0 || ry >= H) continue;
            for (int rx = r.x; rx < r.x + r.l; ++rx) {
                if (rx < 0 || rx >= W) continue;

                if (!testOn) {
                    grid[ry][rx] = r.c;
                    // Test éteint : la profondeur ne change pas
                } else {
                    if (r.z < depth[ry][rx]) {
                        grid[ry][rx] = r.c;
                        depth[ry][rx] = r.z;
                    } else {
                        rejetes++;
                    }
                }
            }
        }
    }

    // Compter les pixels visibles pour chaque lettre dans l'ordre d'apparition
    for (int ry = 0; ry < H; ++ry) {
        for (int rx = 0; rx < W; ++rx) {
            char ch = grid[ry][rx];
            if (ch != '.') {
                visibleCount[ch]++;
            }
        }
    }

    for (int ry = 0; ry < H; ++ry) {
        std::cout << grid[ry] << "\n";
    }

    for (char c : orderLetters) {
        std::cout << c << " " << visibleCount[c] << "\n";
    }

    std::cout << "REJETES " << rejetes << "\n";

    return 0;
}