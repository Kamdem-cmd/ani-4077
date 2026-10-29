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

    const monrhi::Sommet kCarreSansIndex[6] = {
        { -0.20f, -0.20f, 0.0f,  0.96f, 0.62f, 0.14f },
        {  0.20f, -0.20f, 0.0f,  0.20f, 0.75f, 0.70f },
        {  0.20f,  0.20f, 0.0f,  0.90f, 0.30f, 0.35f },
        { -0.20f, -0.20f, 0.0f,  0.96f, 0.62f, 0.14f },
        {  0.20f,  0.20f, 0.0f,  0.90f, 0.30f, 0.35f },
        { -0.20f,  0.20f, 0.0f,  0.95f, 0.95f, 0.95f },
    };

    const monrhi::Sommet kCarreCoins[4] = {
        { -0.20f, -0.20f, 0.0f,  0.96f, 0.62f, 0.14f },
        {  0.20f, -0.20f, 0.0f,  0.20f, 0.75f, 0.70f },
        {  0.20f,  0.20f, 0.0f,  0.90f, 0.30f, 0.35f },
        { -0.20f,  0.20f, 0.0f,  0.95f, 0.95f, 0.95f },
    };
    const unsigned short kCarreIndex[6] = { 0, 1, 2,   0, 2, 3 };

    const monrhi::Sommet kMaisonSansIndex[9] = {
        { -0.30f, -0.30f, 0.0f,  0.50f, 0.50f, 0.50f },
        {  0.30f, -0.30f, 0.0f,  0.50f, 0.50f, 0.50f },
        {  0.30f,  0.10f, 0.0f,  0.50f, 0.50f, 0.50f },
        { -0.30f, -0.30f, 0.0f,  0.50f, 0.50f, 0.50f },
        {  0.30f,  0.10f, 0.0f,  0.50f, 0.50f, 0.50f },
        { -0.30f,  0.10f, 0.0f,  0.50f, 0.50f, 0.50f },
        { -0.30f,  0.10f, 0.0f,  0.80f, 0.20f, 0.20f },
        {  0.30f,  0.10f, 0.0f,  0.80f, 0.20f, 0.20f },
        {  0.00f,  0.40f, 0.0f,  0.80f, 0.20f, 0.20f },
    };

    const monrhi::Sommet kMaisonCoins[5] = {
        { -0.30f, -0.30f, 0.0f,  0.50f, 0.50f, 0.50f },
        {  0.30f, -0.30f, 0.0f,  0.50f, 0.50f, 0.50f },
        {  0.30f,  0.10f, 0.0f,  0.50f, 0.50f, 0.50f },
        { -0.30f,  0.10f, 0.0f,  0.50f, 0.50f, 0.50f },
        {  0.00f,  0.40f, 0.0f,  0.80f, 0.20f, 0.20f },
    };
    const unsigned short kMaisonIndex[9] = { 0, 1, 2,   0, 2, 3,   3, 2, 4 };

} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    NkInitialise();
    NkChrono::BeginPreciseTiming();

    NkWindowConfig cfg;
    cfg.title = "Chapitre 08 - Exo 1 : Le carre des deux facons";
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

    // --- Calculs et affichage des octets (Carré) ---
    size_t octetsCarreSans = sizeof(kCarreSansIndex);
    size_t octetsCarreAvec = sizeof(kCarreCoins) + sizeof(kCarreIndex);
    size_t ecoCarre = octetsCarreSans - octetsCarreAvec;
    float pctCarre = (float)ecoCarre * 100.0f / (float)octetsCarreSans;

    logger.Infof("carre SANS index : 6 sommets, %zu octets", octetsCarreSans);
    logger.Infof("carre AVEC index : 6 index sur 4 sommets, %zu octets", octetsCarreAvec);
    logger.Infof("economie : %zu octets, soit %.1f %%", ecoCarre, pctCarre);

    // --- Calculs et affichage des octets (Maison / Toit) ---
    size_t octetsMaisonSans = sizeof(kMaisonSansIndex);
    size_t octetsMaisonAvec = sizeof(kMaisonCoins) + sizeof(kMaisonIndex);
    size_t ecoMaison = octetsMaisonSans - octetsMaisonAvec;
    float pctMaison = (float)ecoMaison * 100.0f / (float)octetsMaisonSans;

    logger.Infof("maison SANS index : 9 sommets, %zu octets", octetsMaisonSans);
    logger.Infof("maison AVEC index : 9 index sur 5 sommets, %zu octets", octetsMaisonAvec);
    logger.Infof("economie : %zu octets, soit %.2f %%", ecoMaison, pctMaison);

    // Création du maillage indexé pour le carré
    monrhi::Maillage carreIndexe;
    carreIndexe.CreerIndexe(kCarreCoins, 4, kCarreIndex, 6);

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