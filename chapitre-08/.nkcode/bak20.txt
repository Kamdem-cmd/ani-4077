#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKWindow/Core/NkSurface.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"

#include "NKLogger/NkLog.h"
#include "NKPlatform/NkEnv.h"
#include "NKTime/NkChrono.h"
#include "MonRhi/MonRhi.h"
#include "MonRhi/Contexte.h"
#include "MonRhi/Maillage.h"
#include "MonRhi/Nuanceur.h"
#include "MonRhi/Gl.h"

#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;

namespace {
    const float kPetroleR = 0.05f;
    const float kPetroleV = 0.25f;
    const float kPetroleB = 0.28f;

    const char *kSommets = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;
out vec3 vCouleur;
void main() {
    vCouleur = aCouleur;
    gl_Position = vec4(aPosition, 1.0);
}
)";

    const char *kFragments = R"(
in vec3 vCouleur;
out vec4 oCouleur;
void main() {
    oCouleur = vec4(vCouleur, 1.0);
}
)";
}

int nkmain(const NkEntryState &state) {
    (void)state;
    NkInitialise();
    NkChrono::BeginPreciseTiming();

    NkWindowConfig cfg;
    cfg.title = "Chapitre 08 - Exo 4 : La grille";
    cfg.width = 1280;
    cfg.height = 720;
    cfg.centered = true;

    NkWindow fenetre(cfg);
    if (!fenetre.IsValid()) {
        logger.Error("la fenetre ne s'est pas ouverte");
        NkClose();
        return 1;
    }

    const NkSurfaceDesc surface = fenetre.GetSurfaceDesc();
    monrhi::Contexte contexte;
    if (!contexte.Creer(surface)) {
        logger.Error("contexte impossible.");
        NkClose();
        return 1;
    }

    monrhi::Nuanceur nuanceur;
    if (!nuanceur.Compiler(kSommets, kFragments)) {
        contexte.Detruire();
        NkClose();
        return 1;
    }

    // --- 1. Test pour N = 2, 10, 100 ---
    int N_vals[] = {2, 10, 100};
    for (int N : N_vals) {
        int nbSommetsSans = (N - 1) * (N - 1) * 6;
        size_t octetsSans = nbSommetsSans * sizeof(monrhi::Sommet);

        int nbSommetsAvec = N * N;
        int nbIndex = (N - 1) * (N - 1) * 6;
        size_t octetsAvec = nbSommetsAvec * sizeof(monrhi::Sommet) + nbIndex * sizeof(unsigned short);

        float rapport = (float)octetsSans / (float)octetsAvec;
        logger.Infof("N = %d : sans index %zu octets, avec index %zu octets, rapport %.2f", N, octetsSans, octetsAvec, rapport);
    }

    // --- 2. Grande grille (> 70 000 sommets, ex: N = 265 -> 70225 sommets) ---
    int N_grand = 265;
    int totalSommets = N_grand * N_grand;
    int totalIndex = (N_grand - 1) * (N_grand - 1) * 6;

    std::vector<monrhi::Sommet> sommets;
    sommets.reserve(totalSommets);

    for (int y = 0; y < N_grand; ++y) {
        float py = -0.9f + 1.8f * (float)y / (float)(N_grand - 1);
        for (int x = 0; x < N_grand; ++x) {
            float px = -0.9f + 1.8f * (float)x / (float)(N_grand - 1);

            // Colorier les rangées une sur deux d'une couleur franche
            float r = (y % 2 == 0) ? 0.9f : 0.2f;
            float g = (y % 2 == 0) ? 0.3f : 0.8f;
            float b = 0.5f;

            sommets.push_back({px, py, 0.0f, r, g, b});
        }
    }

    std::vector<unsigned int> index32;
    std::vector<unsigned short> index16;
    index32.reserve(totalIndex);
    index16.reserve(totalIndex);

    for (int y = 0; y < N_grand - 1; ++y) {
        for (int x = 0; x < N_grand - 1; ++x) {
            unsigned int i0 = y * N_grand + x;
            unsigned int i1 = y * N_grand + (x + 1);
            unsigned int i2 = (y + 1) * N_grand + (x + 1);
            unsigned int i3 = (y + 1) * N_grand + x;

            // Triangle 1
            index32.push_back(i0); index32.push_back(i1); index32.push_back(i2);
            index16.push_back((unsigned short)i0); index16.push_back((unsigned short)i1); index16.push_back((unsigned short)i2);

            // Triangle 2
            index32.push_back(i0); index32.push_back(i2); index32.push_back(i3);
            index16.push_back((unsigned short)i0); index16.push_back((unsigned short)i2); index16.push_back((unsigned short)i3);
        }
    }

    size_t octetsGrille2 = totalSommets * sizeof(monrhi::Sommet) + totalIndex * sizeof(unsigned short);
    size_t octetsGrille4 = totalSommets * sizeof(monrhi::Sommet) + totalIndex * sizeof(unsigned int);

    logger.Infof("grille %d : %d sommets, index sur 2 octets, %zu octets", N_grand, totalSommets, octetsGrille2);
    logger.Infof("grille %d : %d sommets, index sur 4 octets, %zu octets", N_grand, totalSommets, octetsGrille4);

    // Création des deux maillages (ou un seul que l'on recrée/met à jour, ou deux distincts pour un basculement instantané)
    monrhi::Maillage maillage16bits;
    maillage16bits.CreerIndexe(sommets.data(), totalSommets, index16.data(), totalIndex);

    monrhi::Maillage maillage32bits;
    maillage32bits.CreerIndexe32(sommets.data(), totalSommets, index32.data(), totalIndex);

    bool utiliser32bits = false; // Par défaut, on commence avec les index sur 2 octets (16 bits)
    logger.Infof("Mode actuel : Index sur 2 octets (GL_UNSIGNED_SHORT)");

    bool tourne = true;
    while (tourne && fenetre.IsOpen()) {
        NkEvent *ev = nullptr;
        while (NkEvents().PollEvent(ev)) {
            if (ev->As<NkWindowCloseEvent>() != nullptr) {
                tourne = false;
            }
            // Gestion de l'appui sur une touche pour basculer (ex: touche Espace ou autre selon les événements de ton framework)
            auto keyEvent = ev->As<NkKeyPressEvent>(); // ou l'équivalent dans ton framework pour les touches pressées
            if (keyEvent != nullptr) {
                // Basculement lors d'un appui (par exemple, si tu utilises un événement de touche)
                utiliser32bits = !utiliser32bits;
                if (utiliser32bits) {
                    logger.Infof("Bascule : Index sur 4 octets (GL_UNSIGNED_INT)");
                } else {
                    logger.Infof("Bascule : Index sur 2 octets (GL_UNSIGNED_SHORT)");
                }
            }
        }
        if (!tourne) break;

        if (!contexte.PeutDessiner()) {
            NkChrono::Sleep((int64)16);
            continue;
        }

        const auto taille = fenetre.GetSize();
        glViewport(0, 0, (GLsizei)taille.x, (GLsizei)taille.y);

        glClearColor(kPetroleR, kPetroleV, kPetroleB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        nuanceur.Activer();

        // Dessin selon le mode actif
        if (utiliser32bits) {
            maillage32bits.Dessiner();
        } else {
            maillage16bits.Dessiner();
        }

        contexte.Echanger();
        NkChrono::YieldThread();
    }

    maillage16bits.Detruire();
    maillage32bits.Detruire();

    nuanceur.Detruire();
    contexte.Detruire();
    NkClose();
    return 0;
}