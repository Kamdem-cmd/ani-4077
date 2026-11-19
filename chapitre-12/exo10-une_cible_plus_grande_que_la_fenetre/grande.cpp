#include <iostream>
#include <string>
#include <algorithm>

void resoudreScenario(const std::string& labelScen) {
    long long W = 0, H = 0, L = 0, M = 0, C = 0, D = 0, F = 0;
    if (!(std::cin >> W >> H >> L >> M >> C >> D >> F)) return;

    long long n = L * M;
    long long m = W * H;
    long long rapport = n * 100 / m;

    long long memCouleur = n * C;
    long long memProf = n * D;
    long long memTotale = memCouleur + memProf;
    long long fragPerSec = n * F;

    long long texelsLus = 4 * m;
    long long texelsIgnores = n - 4 * m;
    if (texelsIgnores < 0) texelsIgnores = 0;

    std::string sens = "SENS EGAL";
    if (n > m) sens = "SENS REDUCTION";
    else if (n < m) sens = "SENS AGRANDISSEMENT";

    std::cout << "SCENARIO " << labelScen << "\n";
    std::cout << "CIBLE PIXELS " << n << "\n";
    std::cout << "FENETRE PIXELS " << m << "\n";
    std::cout << "RAPPORT " << rapport << "\n";
    std::cout << "MEMOIRE COULEUR " << memCouleur << "\n";
    std::cout << "MEMOIRE PROFONDEUR " << memProf << "\n";
    std::cout << "MEMOIRE TOTALE " << memTotale << "\n";
    std::cout << "FRAGMENTS PAR SECONDE " << fragPerSec << "\n";
    std::cout << "TEXELS LUS " << texelsLus << "\n";
    std::cout << "TEXELS IGNORES " << texelsIgnores << "\n";
    std::cout << sens << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");
    resoudreScenario("C");

    return 0;
}