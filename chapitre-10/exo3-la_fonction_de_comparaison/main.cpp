#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int W = 0, N = 0;
    if (!(std::cin >> W >> N)) return 0;

    std::string couleur(W, '.');
    std::vector<int> profondeur(W, 100);

    bool testOn = false;
    std::string func = "LESS";
    bool maskOn = true;

    long long ecrits = 0;
    long long profondeursCount = 0;
    long long jetes = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "TEST") {
            std::string state;
            std::cin >> state;
            testOn = (state == "ON");
        } else if (cmd == "FUNC") {
            std::cin >> func;
        } else if (cmd == "MASK") {
            std::string state;
            std::cin >> state;
            maskOn = (state == "ON");
        } else if (cmd == "CLEAR") {
            couleur = std::string(W, '.');
            std::fill(profondeur.begin(), profondeur.end(), 100);
        } else if (cmd == "SEG") {
            char c;
            int debut, longueur, z;
            std::cin >> c >> debut >> longueur >> z;

            for (int k = 0; k < longueur; ++k) {
                int px = debut + k;
                if (px < 0 || px >= W) continue;

                bool passed = true;
                if (testOn) {
                    int d = profondeur[px];
                    if (func == "LESS") passed = (z < d);
                    else if (func == "LEQUAL") passed = (z <= d);
                    else if (func == "GREATER") passed = (z > d);
                    else if (func == "ALWAYS") passed = true;
                    else if (func == "NEVER") passed = false;
                }

                if (passed) {
                    couleur[px] = c;
                    ecrits++;
                    if (testOn && maskOn) {
                        profondeur[px] = z;
                        profondeursCount++;
                    } else if (!testOn) {
                        // Test éteint : la profondeur n'est jamais écrite
                    }
                } else {
                    jetes++;
                }
            }
        }
    }

    std::cout << "COULEUR " << couleur << "\n";
    std::cout << "PROFONDEUR";
    for (int p : profondeur) {
        std::cout << " " << p;
    }
    std::cout << "\n";
    std::cout << "ECRITS " << ecrits << "\n";
    std::cout << "PROFONDEURS " << profondeursCount << "\n";
    std::cout << "JETES " << jetes << "\n";

    return 0;
}