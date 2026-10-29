#include <iostream>
#include <string>
#include <vector>

struct Event {
    int temps;
    std::string type;
};

void run_simulation(const std::vector<Event>& events) {
    // ----------------------------------------------------
    // MODE NAÏF
    // ----------------------------------------------------
    {
        bool surface_vivante = true;
        int current_surface_id = 0;
        int prog_attached_id = 0;

        int images = 0;
        int erreurs = 0;
        int sommeils = 0;
        int rattachements = 0;

        for (const auto& ev : events) {
            if (ev.type == "PERTE") {
                surface_vivante = false;
            } else if (ev.type == "RETOUR") {
                surface_vivante = true;
                current_surface_id++;
            } else if (ev.type == "IMAGE") {
                if (surface_vivante && (prog_attached_id == current_surface_id)) {
                    images++;
                } else {
                    erreurs++;
                }
            }
            // CACHEE et MONTREE sont ignorés en mode naïf
        }

        std::cout << "MODE NAIF\n";
        std::cout << "IMAGES " << images << "\n";
        std::cout << "ERREURS " << erreurs << "\n";
        std::cout << "SOMMEILS " << sommeils << "\n";
        std::cout << "RATTACHEMENTS " << rattachements << "\n";
    }

    // ----------------------------------------------------
    // MODE CORRIGÉ
    // ----------------------------------------------------
    {
        bool surface_vivante = true;
        int current_surface_id = 0;
        int prog_attached_id = 0;
        bool prog_attache = true;

        int images = 0;
        int erreurs = 0;
        int sommeils = 0;
        int rattachements = 0;

        for (const auto& ev : events) {
            if (ev.type == "PERTE") {
                surface_vivante = false;
            } else if (ev.type == "RETOUR") {
                surface_vivante = true;
                current_surface_id++;
            } else if (ev.type == "CACHEE") {
                prog_attache = false;
            } else if (ev.type == "MONTREE") {
                if (!prog_attache && surface_vivante) {
                    prog_attache = true;
                    prog_attached_id = current_surface_id;
                    rattachements++;
                }
            } else if (ev.type == "IMAGE") {
                if (!prog_attache) {
                    sommeils++;
                } else {
                    if (surface_vivante && (prog_attached_id == current_surface_id)) {
                        images++;
                    } else {
                        erreurs++;
                        prog_attache = false; // Se détache immédiatement après l'erreur
                    }
                }
            }
        }

        std::cout << "MODE CORRIGE\n";
        std::cout << "IMAGES " << images << "\n";
        std::cout << "ERREURS " << erreurs << "\n";
        std::cout << "SOMMEILS " << sommeils << "\n";
        std::cout << "RATTACHEMENTS " << rattachements << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        return 0;
    }

    std::vector<Event> events(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> events[i].temps >> events[i].type;
    }

    run_simulation(events);

    return 0;
}