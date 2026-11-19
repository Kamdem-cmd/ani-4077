#include <iostream>
#include <string>
#include <map>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::map<std::string, long long> lastId = {
        {"PROGRAMME", 0}, {"TABLEAU", 0}, {"TEXTURE", 0}, {"CIBLE", 0}
    };

    long long liaisons = 0;
    long long inutiles = 0;
    long long dessins = 0;
    long long elements = 0;
    long long currentSuite = 0;
    long long maxSuite = 0;
    long long rapports = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "LIE") {
            std::string quoi;
            long long id;
            std::cin >> quoi >> id;

            liaisons++;
            if (lastId[quoi] == id) {
                inutiles++;
                currentSuite++;
                maxSuite = std::max(maxSuite, currentSuite);
            } else {
                lastId[quoi] = id;
                currentSuite = 0;
            }
        } else if (cmd == "DESSINE") {
            long long e;
            std::cin >> e;
            dessins++;
            elements += e;
        } else if (cmd == "RAPPORT") {
            rapports++;
            long long part = (liaisons == 0) ? 0 : (inutiles * 100 / liaisons);
            std::cout << "LIAISONS " << liaisons 
                      << " INUTILES " << inutiles 
                      << " PART " << part << " %"
                      << " DESSINS " << dessins 
                      << " ELEMENTS " << elements 
                      << " SUITE " << maxSuite << "\n";
        } else if (cmd == "REINIT") {
            lastId["PROGRAMME"] = 0;
            lastId["TABLEAU"] = 0;
            lastId["TEXTURE"] = 0;
            lastId["CIBLE"] = 0;
            liaisons = 0;
            inutiles = 0;
            dessins = 0;
            elements = 0;
            currentSuite = 0;
            maxSuite = 0;
        }
    }

    std::cout << "RAPPORTS " << rapports << "\n";

    return 0;
}