#include <iostream>
#include <string>
#include <vector>
#include <cctype>

bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool containsOpenGLName(const std::string& line) {
    size_t n = line.size();
    for (size_t i = 0; i < n; ++i) {
        if (i > 0 && isWordChar(line[i - 1])) {
            continue;
        }
        // Vérification gl suivi d'une majuscule
        if (i + 2 < n && line[i] == 'g' && line[i + 1] == 'l' &&
            std::isupper(static_cast<unsigned char>(line[i + 2]))) {
            return true;
        }
        // Vérification GL_
        if (i + 2 < n && line[i] == 'G' && line[i + 1] == 'L' && line[i + 2] == '_') {
            return true;
        }
    }
    return false;
}

bool endsWithHeader(const std::string& filename) {
    return filename.size() >= 2 && filename.substr(filename.size() - 2) == ".h";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int F = 0;
    if (!(std::cin >> F)) return 0;

    long long entetesLignes = 0, entetesGL = 0;
    long long sourcesLignes = 0, sourcesGL = 0;
    long long cacheesTotal = 0;

    std::string plusLourdNom = "AUCUN";
    long long maxGL = 0;

    for (int f = 0; f < F; ++f) {
        std::string tag, nom;
        long long K = 0;
        std::cin >> tag >> nom >> K;
        std::string dummy;
        std::getline(std::cin, dummy); // Consommer la fin de ligne

        long long g = 0;
        long long r = 0;

        for (long long k = 0; k < K; ++k) {
            std::string ligne;
            std::getline(std::cin, ligne);

            size_t commentPos = ligne.find("//");
            bool hasComment = (commentPos != std::string::npos);

            std::string codePart = hasComment ? ligne.substr(0, commentPos) : ligne;

            bool justeMatch = containsOpenGLName(codePart);
            bool grepMatch = (!hasComment) && containsOpenGLName(ligne);

            if (justeMatch) g++;
            if (grepMatch) r++;
            if (justeMatch && !grepMatch) cacheesTotal++;
        }

        std::cout << nom << " LIGNES " << K << " GL " << g << " GREP " << r << "\n";

        if (endsWithHeader(nom)) {
            entetesLignes += K;
            entetesGL += g;
        } else {
            sourcesLignes += K;
            sourcesGL += g;
        }

        if (g > maxGL) {
            maxGL = g;
            plusLourdNom = nom;
        }
    }

    std::cout << "ENTETES LIGNES " << entetesLignes << " GL " << entetesGL << "\n";
    std::cout << "SOURCES LIGNES " << sourcesLignes << " GL " << sourcesGL << "\n";
    std::cout << "PLUS LOURD " << plusLourdNom << "\n";
    std::cout << "CACHEES " << cacheesTotal << "\n";

    return 0;
}