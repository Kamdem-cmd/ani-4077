#include <iostream>
#include <string>
#include <map>

struct VAOState {
    int currentEBO = 0;
    int expectedEBO = 0;
};

void resoudreScenario(const std::string& labelScen) {
    int N = 0, D = 0;
    if (!(std::cin >> N >> D)) return;

    int boundVAO = 0;
    std::map<int, VAOState> vaos;

    long long appels = 0;
    long long dessinNum = 0;
    long long fauxCount = 0;

    std::cout << "SCENARIO " << labelScen << "\n";

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "CREE") {
            int v, e;
            std::cin >> v >> e;
            // Lie v (1 appel), attache e, puis lie 0 (1 appel)
            appels++;
            boundVAO = v;
            vaos[v].currentEBO = e;
            vaos[v].expectedEBO = e;
            appels++;
            boundVAO = 0;
        } else if (cmd == "TELEVERSE") {
            int e;
            std::cin >> e;
            if (boundVAO != 0) {
                vaos[boundVAO].currentEBO = e;
            }
        } else if (cmd == "DESSINE") {
            int v;
            std::cin >> v;
            dessinNum++;

            // Lie v (1 appel)
            appels++;
            boundVAO = v;

            int lu = vaos[v].currentEBO;
            int attendu = vaos[v].expectedEBO;

            if (lu == attendu) {
                std::cout << "DESSIN " << dessinNum << " " << v << " JUSTE\n";
            } else {
                std::cout << "DESSIN " << dessinNum << " " << v << " FAUX INDEX " << lu << "/" << attendu << "\n";
                fauxCount++;
            }

            if (D == 1) {
                appels++;
                boundVAO = 0;
            }
        }
    }

    long long corrompus = 0;
    for (const auto& [id, state] : vaos) {
        if (state.currentEBO != state.expectedEBO) {
            corrompus++;
        }
    }

    std::cout << "APPELS " << appels << "\n";
    std::cout << "CORROMPUS " << corrompus << "\n";
    std::cout << "FAUX " << fauxCount << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");

    return 0;
}