#include <iostream>
#include <string>
#include <vector>
#include <cctype>

bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool isForbiddenWord(const std::string& word) {
    if (word.find("GL_") != std::string::npos) return true;
    if (word.find("Camera") != std::string::npos) return true;
    if (word.find("Scene") != std::string::npos) return true;
    for (size_t i = 0; i + 2 < word.size(); ++i) {
        if (word[i] == 'g' && word[i + 1] == 'l' &&
            std::isupper(static_cast<unsigned char>(word[i + 2]))) {
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::string dummy;
    std::getline(std::cin, dummy);

    bool inBlockComment = false;
    long long codeLines = 0;
    long long forbiddenCount = 0;

    for (int lineNum = 1; lineNum <= N; ++lineNum) {
        std::string rawLine;
        std::getline(std::cin, rawLine);

        std::string codeLine;
        size_t len = rawLine.size();
        size_t i = 0;

        while (i < len) {
            if (inBlockComment) {
                if (i + 1 < len && rawLine[i] == '*' && rawLine[i + 1] == '/') {
                    inBlockComment = false;
                    codeLine.push_back(' '); // compte comme une seule espace
                    i += 2;
                } else {
                    i++;
                }
            } else {
                if (i + 1 < len && rawLine[i] == '/' && rawLine[i + 1] == '*') {
                    inBlockComment = true;
                    i += 2;
                } else if (i + 1 < len && rawLine[i] == '/' && rawLine[i + 1] == '/') {
                    break; // commentaire jusqu'à la fin de la ligne
                } else {
                    codeLine.push_back(rawLine[i]);
                    i++;
                }
            }
        }

        bool hasNonSpace = false;
        for (char c : codeLine) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                hasNonSpace = true;
                break;
            }
        }
        if (hasNonSpace) {
            codeLines++;
        }

        // Extraction des mots
        size_t cLen = codeLine.size();
        size_t pos = 0;
        while (pos < cLen) {
            if (isWordChar(codeLine[pos])) {
                size_t start = pos;
                while (pos < cLen && isWordChar(codeLine[pos])) {
                    pos++;
                }
                std::string word = codeLine.substr(start, pos - start);
                if (isForbiddenWord(word)) {
                    std::cout << "LIGNE " << lineNum << " " << word << "\n";
                    forbiddenCount++;
                }
            } else {
                pos++;
            }
        }
    }

    std::cout << "LIGNES DE CODE " << codeLines << "\n";
    std::cout << "INTERDITS " << forbiddenCount << "\n";
    std::cout << "VERDICT " << (forbiddenCount == 0 ? "PROPRE" : "REFUSE") << "\n";

    return 0;
}