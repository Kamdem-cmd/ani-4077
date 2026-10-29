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
    // Version 1 : uInutile déclaré mais jamais employé
    const char *kSommetsV1 = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;
uniform vec2 uDecalage;
uniform vec3 uTeinte;
uniform vec3 uInutile;
out vec3 vCouleur;
void main() {
    vCouleur = aCouleur * uTeinte;
    gl_Position = vec4(aPosition.xy + uDecalage, aPosition.z, 1.0);
}
)";

    // Version 2 : uInutile employé avec un coefficient nul (0.0)
    const char *kSommetsV2 = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;
uniform vec2 uDecalage;
uniform vec3 uTeinte;
uniform vec3 uInutile;
out vec3 vCouleur;
void main() {
    vCouleur = aCouleur * uTeinte + 0.0 * uInutile;
    gl_Position = vec4(aPosition.xy + uDecalage, aPosition.z, 1.0);
}
)";

    // Version 3 : uInutile employé avec un coefficient non nul (0.001)
    const char *kSommetsV3 = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;
uniform vec2 uDecalage;
uniform vec3 uTeinte;
uniform vec3 uInutile;
out vec3 vCouleur;
void main() {
    vCouleur = aCouleur * uTeinte + 0.001 * uInutile;
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
        { -0.20f, -0.20f, 0.0f,  0.96f, 0.62f, 0.14f },
        {  0.20f, -0.20f, 0.0f,  0.20f, 0.75f, 0.70f },
        {  0.20f,  0.20f, 0.0f,  0.90f, 0.30f, 0.35f },
        { -0.20f,  0.20f, 0.0f,  0.95f, 0.95f, 0.95f },
    };
    const unsigned short kCarreIndex[6] = { 0, 1, 2,   0, 2, 3 };
}

int nkmain(const NkEntryState &state) {
    (void)state;
    NkInitialise();
    NkChrono::BeginPreciseTiming();

    NkWindowConfig cfg;
    cfg.title = "Chapitre 08 - Exo 5 : L'uniforme qui disparait";
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

    // Fonction utilitaire pour tester et logger les emplacements de chaque version
    auto testerVersion = [](const char* sourceSommets, const char* label) {
        monrhi::Nuanceur nu;
        if (nu.Compiler(sourceSommets, kFragments)) {
            GLint locDecalage = glGetUniformLocation(nu.Id(), "uDecalage");
            GLint locTeinte = glGetUniformLocation(nu.Id(), "uTeinte");
            GLint locInutile = glGetUniformLocation(nu.Id(), "uInutile");
            logger.Infof("%s : uDecalage %d, uTeinte %d, uInutile %d", label, locDecalage, locTeinte, locInutile);
            nu.Detruire();
        }
    };

    testerVersion(kSommetsV1, "jamais employe");
    testerVersion(kSommetsV2, "coefficient nul");
    testerVersion(kSommetsV3, "coefficient 0.001");

    // Lancement de la boucle de rendu avec la version finale
    monrhi::Nuanceur nuanceur;
    if (!nuanceur.Compiler(kSommetsV3, kFragments)) {
        contexte.Detruire();
        NkClose();
        return 1;
    }

    monrhi::Maillage carreIndexe;
    carreIndexe.CreerIndexe(kCarreCoins, 4, kCarreIndex, 6);

    bool tourne = true;
    while (tourne && fenetre.IsOpen()) {
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
        carreIndexe.Dessiner();

        contexte.Echanger();
        NkChrono::YieldThread();
    }

    carreIndexe.Detruire();
    nuanceur.Detruire();
    contexte.Detruire();
    NkClose();
    return 0;
}