#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    long long reel = 0;
    long long retenu = 0;
    bool cacheConnu = true;

    long long appels = 0;
    long long evites = 0;
    long long directs = 0;
    long long faux = 0;
    long long dessinNum = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "ACTIVE") {
            long long p;
            std::cin >> p;
            if (cacheConnu && retenu == p) {
                evites++;
            } else {
                appels++;
                reel = p;
                retenu = p;
                cacheConnu = true;
            }
        } else if (cmd == "DIRECT") {
            long long p;
            std::cin >> p;
            reel = p;
            directs++;
        } else if (cmd == "OUBLIE") {
            cacheConnu = false;
        } else if (cmd == "DESSINE") {
            long long p;
            std::cin >> p;
            dessinNum++;
            if (reel == p) {
                std::cout << "DESSIN " << dessinNum << " JUSTE\n";
            } else {
                std::cout << "DESSIN " << dessinNum << " FAUX " << reel << "/" << p << "\n";
                faux++;
            }
        }
    }

    std::cout << "APPELS " << appels << "\n";
    std::cout << "EVITES " << evites << "\n";
    std::cout << "DIRECTS " << directs << "\n";
    std::cout << "FAUX " << faux << "\n";

    return 0;
}