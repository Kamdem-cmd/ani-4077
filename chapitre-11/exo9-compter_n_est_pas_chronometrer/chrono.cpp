#include <iostream>
#include <string>
#include <vector>

struct Machine {
    std::string nom;
    long long cp, ct, ca, cd;
};

void resoudreScenario(const std::string& labelScen) {
    int K = 0;
    if (!(std::cin >> K)) return;

    std::vector<Machine> machines(K);
    for (int i = 0; i < K; ++i) {
        std::cin >> machines[i].nom >> machines[i].cp >> machines[i].ct >> machines[i].ca >> machines[i].cd;
    }

    std::string tag;
    long long cp_c, ct_c, ca_c, cd_c;
    long long cp_r, ct_r, ca_r, cd_r;

    std::cin >> tag >> cp_c >> ct_c >> ca_c >> cd_c;
    std::cin >> tag >> cp_r >> ct_r >> ca_r >> cd_r;

    long long x = cp_c + ct_c + ca_c;
    long long y = cp_r + ct_r + ca_r;
    long long e = (x == 0) ? 0 : ((x - y) * 100 / x);

    std::cout << "SCENARIO " << labelScen << "\n";
    std::cout << "LIAISONS CREATION " << x << " REGROUPE " << y << " ECONOMIE " << e << " %\n";

    for (const auto& m : machines) {
        long long tc = cp_c * m.cp + ct_c * m.ct + ca_c * m.ca + cd_c * m.cd;
        long long tr = cp_r * m.cp + ct_r * m.ct + ca_r * m.ca + cd_r * m.cd;
        long long f = (tc == 0) ? 0 : ((tc - tr) * 100 / tc);
        long long g = f - e;

        std::cout << m.nom << " CREATION " << tc << " REGROUPE " << tr 
                  << " ECONOMIE " << f << " % ECART " << g << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    resoudreScenario("A");
    resoudreScenario("B");

    return 0;
}