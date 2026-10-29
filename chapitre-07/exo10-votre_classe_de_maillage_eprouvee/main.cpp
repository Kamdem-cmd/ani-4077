#include <iostream>
#include <string>
#include <vector>

struct Buffer {
    std::vector<std::string> elements;
    bool complet;

    Buffer() {
        elements = {"noir"};
        complet = true;
    }

    void effacer(const std::string& couleur) {
        elements.clear();
        elements.push_back(couleur);
        complet = false;
    }

    void dessiner(const std::string& objet) {
        elements.push_back(objet);
        complet = false;
    }

    std::string get_contenu() const {
        std::string res;
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) res += "+";
            res += elements[i];
        }
        return res;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string mode;
    unsigned long long largeur = 0, hauteur = 0, bits = 0;

    if (!(std::cin >> mode >> largeur >> hauteur >> bits)) {
        return 0;
    }

    // Calcul de la mémoire sur des entiers 64 bits
    unsigned long long memoire = (largeur * hauteur * bits) / 8ULL;
    if (mode == "DOUBLE") {
        memoire *= 2ULL;
    }

    std::cout << "MEMOIRE " << memoire << "\n";

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "INCOMPLETES 0\n";
        return 0;
    }

    Buffer front_buf;
    Buffer back_buf;
    int incompletes = 0;

    for (int i = 0; i < n; ++i) {
        std::string op;
        std::cin >> op;

        if (op == "EFFACER") {
            std::string couleur;
            std::cin >> couleur;
            if (mode == "SIMPLE") {
                front_buf.effacer(couleur);
            } else {
                back_buf.effacer(couleur);
            }
        } 
        else if (op == "DESSINER") {
            std::string objet;
            std::cin >> objet;
            if (mode == "SIMPLE") {
                front_buf.dessiner(objet);
            } else {
                back_buf.dessiner(objet);
            }
        } 
        else if (op == "ECHANGER") {
            if (mode == "SIMPLE") {
                front_buf.complet = true;
            } else {
                back_buf.complet = true;
                std::swap(front_buf, back_buf);
            }
        } 
        else if (op == "AFFICHER") {
            std::string etat_str = front_buf.complet ? "COMPLETE" : "INCOMPLETE";
            if (!front_buf.complet) {
                incompletes++;
            }
            std::cout << "ECRAN " << front_buf.get_contenu() << " " << etat_str << "\n";
        }
    }

    std::cout << "INCOMPLETES " << incompletes << "\n";

    return 0;
}