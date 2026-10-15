#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Format {
    int numero;
    int couleur;
    int profondeur;
    int pochoir;
    int double_tampon;
};

std::string get_etat(int demande, int offert) {
    if (offert == demande) return "EXACT";
    if (offert < demande) return "RABOTE";
    return "PLUS";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int req_c = 0, req_p = 0, req_s = 0, req_dbl = 0;
    if (!(std::cin >> req_c >> req_p >> req_s >> req_dbl)) {
        std::cout << "AUCUN FORMAT\n";
        return 0;
    }

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "AUCUN FORMAT\n";
        return 0;
    }

    bool candidate_found = false;
    Format best_fmt;
    int best_manque = 0;
    int best_exces = 0;

    for (int i = 0; i < n; ++i) {
        Format fmt;
        std::cin >> fmt.numero >> fmt.couleur >> fmt.profondeur >> fmt.pochoir >> fmt.double_tampon;

        // Seuls les formats dont double tampon est égal à celui demandé sont candidats
        if (fmt.double_tampon != req_dbl) {
            continue;
        }

        int manque = 0;
        manque += (fmt.couleur < req_c) ? (req_c - fmt.couleur) : 0;
        manque += (fmt.profondeur < req_p) ? (req_p - fmt.profondeur) : 0;
        manque += (fmt.pochoir < req_s) ? (req_s - fmt.pochoir) : 0;

        int exces = 0;
        exces += (fmt.couleur > req_c) ? (fmt.couleur - req_c) : 0;
        exces += (fmt.profondeur > req_p) ? (fmt.profondeur - req_p) : 0;
        exces += (fmt.pochoir > req_s) ? (fmt.pochoir - req_s) : 0;

        if (!candidate_found) {
            candidate_found = true;
            best_fmt = fmt;
            best_manque = manque;
            best_exces = exces;
        } else {
            if (manque < best_manque) {
                best_fmt = fmt;
                best_manque = manque;
                best_exces = exces;
            } else if (manque == best_manque) {
                if (exces < best_exces) {
                    best_fmt = fmt;
                    best_exces = exces;
                } else if (exces == best_exces) {
                    if (fmt.numero < best_fmt.numero) {
                        best_fmt = fmt;
                    }
                }
            }
        }
    }

    if (!candidate_found) {
        std::cout << "AUCUN FORMAT\n";
        return 0;
    }

    std::string etat_c = get_etat(req_c, best_fmt.couleur);
    std::string etat_p = get_etat(req_p, best_fmt.profondeur);
    std::string etat_s = get_etat(req_s, best_fmt.pochoir);

    int rabotes = 0;
    if (etat_c == "RABOTE") rabotes++;
    if (etat_p == "RABOTE") rabotes++;
    if (etat_s == "RABOTE") rabotes++;

    std::cout << "FORMAT " << best_fmt.numero << "\n";
    std::cout << "couleur " << req_c << " " << best_fmt.couleur << " " << etat_c << "\n";
    std::cout << "profondeur " << req_p << " " << best_fmt.profondeur << " " << etat_p << "\n";
    std::cout << "pochoir " << req_s << " " << best_fmt.pochoir << " " << etat_s << "\n";
    std::cout << "RABOTES " << rabotes << "\n";

    return 0;
}