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

    const monrhi::Sommet kCarreCoins[4] = {
        { -0.20f, -0.20f, 0.0f,  0.96f, 0.62f, 0.14f },
        {  0.20f, -0.20f, 0.0f,  0.20f, 0.75f, 0.70f },
        {  0.20f,  0.20f, 0.0f,  0.90f, 0.30f, 0.35f },
        { -0.20f,  0.20f, 0.0f,  0.95f, 0.95f, 0.95f },
    };

    // MODIFIER CET TABLEAU SELON L'ORDRE TESTÉ :
    // - Test 1 : { 0, 1, 2,   0, 2, 3 }
    // - Test 2 : { 0, 1, 2,   2, 3, 0 }
    // - Test 3 : { 0, 2, 1,   0, 3, 2 }
    const unsigned short kCarreIndex[6] = { 0, 2, 1,   0, 3, 2 };

} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    NkInitialise();
    NkChrono::BeginPreciseTiming();

    NkWindowConfig cfg;
    cfg.title = "Chapitre 08 - Exo 2 : L'ordre des index";
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
    logger.Infof("%s", monrhi::DecrireSurface(surface).CStr());

    monrhi::Contexte contexte;
    if (!contexte.Creer(surface)) {
        logger.Error("contexte impossible : on s'arrete ici.");
        NkClose();
        return 1;
    }
    logger.Infof("contexte : %s", contexte.Decrire().CStr());

    monrhi::Nuanceur nuanceur;
    if (!nuanceur.Compiler(kSommets, kFragments)) {
        logger.Errorf("NUANCEUR REFUSE :\n%s", nuanceur.Erreur().CStr());
        contexte.Detruire();
        NkClose();
        return 1;
    }

    monrhi::Maillage carreIndexe;
    carreIndexe.CreerIndexe(kCarreCoins, 4, kCarreIndex, 6);

    // DÉCOMMENTER LA LIGNE CI-DESSOUS POUR LES ESSAIS AVEC TRI :
    // glEnable(GL_CULL_FACE);

    int numeroImage = 0;
    bool tourne = true;
    while (tourne && fenetre.IsOpen()) {
        NkEvent *ev = nullptr;
        while (NkEvents().PollEvent(ev)) {
            if (ev->As<NkWindowCloseEvent>() != nullptr)
                tourne = false;
            if (ev->As<NkWindowSurfaceDestroyedEvent>() != nullptr || ev->As<NkWindowHiddenEvent>() != nullptr)
                contexte.DetacherSurface();
            if (ev->As<NkWindowSurfaceCreatedEvent>() != nullptr || ev->As<NkWindowShownEvent>() != nullptr)
                contexte.RattacherSurface(fenetre.GetSurfaceDesc());
        }
        if (!tourne)
            break;

        if (!contexte.PeutDessiner()) {
            NkChrono::Sleep((int64)16);
            continue;
        }

        const auto taille = fenetre.GetSize();
        glViewport(0, 0, (GLsizei)taille.x, (GLsizei)taille.y);

        glClearColor(kPetroleR, kPetroleV, kPetroleB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        nuanceur.Activer();
        carreIndexe.Dessiner();

        numeroImage++;
        if (numeroImage == 3) {
            unsigned char px[4];
            glReadPixels(640, 360, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);

            // Adapter la chaîne selon l'ordre et le mode testés pour récupérer la ligne dans app.log
            logger.Infof("ordre {0,1,2,  0,3,2}, sans tri : centre (%u, %u, %u)", px[0], px[1], px[2]);
        }

        contexte.Echanger();
        NkChrono::YieldThread();
    }

    carreIndexe.Detruire();
    nuanceur.Detruire();
    contexte.Detruire();
    NkChrono::EndPreciseTiming();
    NkClose();
    return 0;
}
