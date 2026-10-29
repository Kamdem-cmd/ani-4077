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
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;

namespace {
    const char *kSommets = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;
uniform vec2 uDecalage;
uniform vec3 uTeinte;
out vec3 vCouleur;
void main() {
    vCouleur = aCouleur * uTeinte;
    gl_Position = vec4(aPosition.xy + uDecalage, aPosition.z, 1.0);
}
)";

    const char *kFragments = R"(
in vec3 vCouleur;
out vec4 oCouleur;
void main() {
    oCouleur = vec4(vCouleur, 1.0);
}
)";

    const monrhi::Sommet kCarreCoins[4] = {
        { -0.08f, -0.08f, 0.0f,  0.96f, 0.62f, 0.14f },
        {  0.08f, -0.08f, 0.0f,  0.20f, 0.75f, 0.70f },
        {  0.08f,  0.08f, 0.0f,  0.90f, 0.30f, 0.35f },
        { -0.08f,  0.08f, 0.0f,  0.95f, 0.95f, 0.95f },
    };
    const unsigned short kCarreIndex[6] = { 0, 1, 2,   0, 2, 3 };
}

int nkmain(const NkEntryState &state) {
    (void)state;
    NkInitialise();
    NkChrono::BeginPreciseTiming();

    NkWindowConfig cfg;
    cfg.title = "Chapitre 08 - Exo 6 : Cent objets";
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

    // Retrait de la vérification Glad pour les mesures
    gladUninstallGLDebug();

    monrhi::Nuanceur nuanceur;
    if (!nuanceur.Compiler(kSommets, kFragments)) {
        contexte.Detruire();
        NkClose();
        return 1;
    }

    // Lecture de la variable d'environnement pour choisir la version
    // $env:CENT = "1" pour cent tampons, sinon un tampon unique
    const char *envCent = env::GetEnvVar("CENT");
    bool centTampons = (envCent != nullptr && std::string(envCent) == "1");

    size_t octetsMaillages = 0;
    monrhi::Maillage maillageUnique;
    std::vector<monrhi::Maillage*> listeMaillages;

    if (!centTampons) {
        maillageUnique.CreerIndexe(kCarreCoins, 4, kCarreIndex, 6);
        octetsMaillages = 4 * sizeof(monrhi::Sommet) + 6 * sizeof(unsigned short);
    } else {
        listeMaillages.resize(100);
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                float x = -0.8f + (float)j * 0.18f;
                float y = -0.8f + (float)i * 0.18f;

                // Si les champs de couleur s'appellent différemment ou si tu veux juste utiliser les teintes de base :
                monrhi::Sommet coins[4];
                for (int v = 0; v < 4; ++v) {
                    coins[v].x = kCarreCoins[v].x + x;
                    coins[v].y = kCarreCoins[v].y + y;
                    coins[v].z = kCarreCoins[v].z;
                    // Copie directe des attributs du sommet de base pour éviter tout problème de nommage
                    coins[v].r = kCarreCoins[v].r;
                    coins[v].g = kCarreCoins[v].g;
                    coins[v].b = kCarreCoins[v].b;
                }
                int idx = i * 10 + j;
                listeMaillages[idx] = new monrhi::Maillage();
                listeMaillages[idx]->CreerIndexe(coins, 4, kCarreIndex, 6);
            }
        }
        octetsMaillages = 100 * (4 * sizeof(monrhi::Sommet) + 6 * sizeof(unsigned short));
    }
        octetsMaillages = 100 * (4 * sizeof(monrhi::Sommet) + 6 * sizeof(unsigned short));

    int frameCount = 0;
    double totalFrameTimeMs = 0.0;
    double totalDrawTimeMs = 0.0;

    bool tourne = true;
    while (tourne && fenetre.IsOpen() && frameCount < 500) {
        int64 frameStart = NkChrono::Now().nanoseconds;

        NkEvent *ev = nullptr;
        while (NkEvents().PollEvent(ev)) {
            if (ev->As<NkWindowCloseEvent>() != nullptr) tourne = false;
        }
        if (!tourne) break;

        if (!contexte.PeutDessiner()) {
            NkChrono::Sleep((int64)16);
            continue;
        }

        const auto taille = fenetre.GetSize();
        glViewport(0, 0, (GLsizei)taille.x, (GLsizei)taille.y);

        glClearColor(0.05f, 0.25f, 0.28f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        nuanceur.Activer();

        int64 drawStart = NkChrono::Now().nanoseconds;

        if (!centTampons) {
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    float x = -0.8f + (float)j * 0.18f;
                    float y = -0.8f + (float)i * 0.18f;
                    float r = (float)i / 9.0f;
                    float g = (float)j / 9.0f;
                    float b = 0.5f;

                    nuanceur.PoserVec2("uDecalage", x, y);
                    nuanceur.PoserVec3("uTeinte", r, g, b);
                    maillageUnique.Dessiner();
                }
            }
        } else {
                nuanceur.PoserVec2("uDecalage", 0.0f, 0.0f);
                nuanceur.PoserVec3("uTeinte", 1.0f, 1.0f, 1.0f);
                for (int k = 0; k < 100; ++k) {
                    listeMaillages[k]->Dessiner();
                }
            }

        int64 drawEnd = NkChrono::Now().nanoseconds;
        double drawMs = (double)(drawEnd - drawStart) / 1000000.0;

        contexte.Echanger();

        int64 frameEnd = NkChrono::Now().nanoseconds;
        double frameMs = (double)(frameEnd - frameStart) / 1000000.0;

        frameCount++;
        if (frameCount > 30) { // Écarter les 30 premières images
            totalFrameTimeMs += frameMs;
            totalDrawTimeMs += drawMs;
        }

        NkChrono::YieldThread();
    }

    int measuredFrames = 500 - 30;
    double avgFrameMs = totalFrameTimeMs / measuredFrames;
    double avgDrawMs = totalDrawTimeMs / measuredFrames;

    if (!centTampons) {
        logger.Infof("un tampon : 100 objets, %.2f ms par image, dessin %.2f ms, %zu octets", avgFrameMs, avgDrawMs, octetsMaillages);
    } else {
        logger.Infof("cent tampons : 100 objets, %.2f ms par image, dessin %.2f ms, %zu octets", avgFrameMs, avgDrawMs, octetsMaillages);
    }

    if (!centTampons) {
        maillageUnique.Detruire();
    } else {
        for (auto &m : listeMaillages) m->Detruire();
    }
    nuanceur.Detruire();
    contexte.Detruire();
    NkClose();
    return 0;
}