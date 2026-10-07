#include <iostream>
#include <string>

int parse_version_code(const std::string& str) {
    size_t dot_pos = str.find('.');
    if (dot_pos == std::string::npos) {
        return 0;
    }

    int majeur = std::stoi(str.substr(0, dot_pos));
    int mineur = str[dot_pos + 1] - '0';

    return majeur * 10 + mineur;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "ACCEPTABLES 0\n";
        std::cout << "SUSPECTS 0\n";
        std::cout << "REFUSES 0\n";
        return 0;
    }

    int acceptables = 0;
    int suspects = 0;
    int refuses = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom, chemin, demande_str, rapporte_str;
        std::cin >> nom >> chemin >> demande_str >> rapporte_str;

        int code_demande = parse_version_code(demande_str);
        int code_rapporte = parse_version_code(rapporte_str);

        std::string verdict;

        if (code_rapporte < code_demande) {
            verdict = "TROP VIEUX";
            refuses++;
        } else if (chemin == "WGL" && code_rapporte > code_demande) {
            verdict = "CONTEXTE PROVISOIRE";
            suspects++;
        } else if (code_rapporte == code_demande) {
            verdict = "EXACT";
            acceptables++;
        } else {
            verdict = "ACCEPTE";
            acceptables++;
        }

        std::cout << nom << " " << code_demande << " " << code_rapporte << " " << verdict << "\n";
    }

    std::cout << "ACCEPTABLES " << acceptables << "\n";
    std::cout << "SUSPECTS " << suspects << "\n";
    std::cout << "REFUSES " << refuses << "\n";

    return 0;
}