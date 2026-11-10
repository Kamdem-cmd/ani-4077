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
#include <cstring>

using namespace nkentseu;

namespace {
    // Nuanceur standard pour la référence (glUniform)
    const char *kSommetsUniform = R"(
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

    // Nuanceur avec Uniform Block (std140)
    const char *kSommetsUBO = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;
layout(std140) uniform Objet {
    vec2 uDecalage;
    vec3 uTeinte;
};
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
    cfg.title = "Chapitre 08 - Exo 10 : Tampon d'uniformes";
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

    // Retrait de Glad pour les mesures
    gladUninstallGLDebug();

    // Lecture de la variable d'environnement pour choisir la version ("uniform", "ubo", "serre")
    const char *envMode = env::GetEnvVar("MODE");
    std::string mode = (envMode != nullptr) ? std::string(envMode) : "uniform";

    monrhi::Nuanceur nuanceur;
    if (mode == "uniform") {
        if (!nuanceur.Compiler(kSommetsUniform, kFragments)) {
            contexte.Detruire();
            NkClose();
            return 1;
        }
    } else {
        if (!nuanceur.Compiler(kSommetsUBO, kFragments)) {
            contexte.Detruire();
            NkClose();
            return 1;
        }
        // Liaison du bloc uniform
        GLuint blockIndex = glGetUniformBlockIndex(nuanceur.Id(), "Objet");
        if (blockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(nuanceur.Id(), blockIndex, 0);
        }
    }

    monrhi::Maillage maillageUnique;
    maillageUnique.CreerIndexe(kCarreCoins, 4, kCarreIndex, 6);

    // Lecture de l'alignement requis pour les UBO
    GLint alignment = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
    if (alignment < 1) alignment = 256; // Valeur de repli sécurisée

    // Calcul de la taille d'un objet selon les règles std140 ou la version serrée
    // std140 : vec2 (8 octets) + padding (8 octets) + vec3 (12 octets) + padding (4 octets) = 32 octets par bloc aligné
    size_t tailleObjetStd140 = 32;
    size_t tailleObjetSerre = sizeof(float) * 5; // vec2 + vec3 collés sans trou (20 octets)
    size_t taillePas = (mode == "serre") ? tailleObjetSerre : (size_t)alignment;

    // Allocation du buffer UBO pour 100 objets
    GLuint ubo = 0;
    std::vector<uint8_t> bufferData;
    if (mode != "uniform") {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);

        size_t bufferSize = 100 * taillePas;
        bufferData.resize(bufferSize, 0);

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                int index = i * 10 + j;
                size_t offset = index * taillePas;

                float x = -0.8f + (float)j * 0.18f;
                float y = -0.8f + (float)i * 0.18f;
                float r = (float)i / 9.0f;
                float g = (float)j / 9.0f;
                float b = 0.5f;

                if (mode == "ubo") {
                    // Respect strict std140 : uDecalage à l'offset, uTeinte à offset + 16
                    std::memcpy(&bufferData[offset], &x, sizeof(float) * 2);
                    std::memcpy(&bufferData[offset + 16], &r, sizeof(float) * 3);
                } else if (mode == "serre") {
                    // Version sans trou : uTeinte collée directement après uDecalage (à l'octet 8)
                    std::memcpy(&bufferData[offset], &x, sizeof(float) * 2);
                    std::memcpy(&bufferData[offset + 8], &r, sizeof(float) * 3);
                }
            }
        }

        glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr)bufferSize, bufferData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

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

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                int index = i * 10 + j;

                if (mode == "uniform") {
                    float x = -0.8f + (float)j * 0.18f;
                    float y = -0.8f + (float)i * 0.18f;
                    float r = (float)i / 9.0f;
                    float g = (float)j / 9.0f;
                    float b = 0.5f;

                    GLint locDec = glGetUniformLocation(nuanceur.Id(), "uDecalage");
                    GLint locTeinte = glGetUniformLocation(nuanceur.Id(), "uTeinte");
                    if (locDec >= 0) glUniform2f(locDec, x, y);
                    if (locTeinte >= 0) glUniform3f(locTeinte, r, g, b);
                } else {
                    GLintptr offset = (GLintptr)(index * taillePas);
                    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, offset, tailleObjetStd140);
                }

                maillageUnique.Dessiner();
            }
        }

        int64 drawEnd = NkChrono::Now().nanoseconds;
        double drawMs = (double)(drawEnd - drawStart) / 1000000.0;

        contexte.Echanger();

        int64 frameEnd = NkChrono::Now().nanoseconds;
        double frameMs = (double)(frameEnd - frameStart) / 1000000.0;

        frameCount++;
        if (frameCount > 30) {
            totalFrameTimeMs += frameMs;
            totalDrawTimeMs += drawMs;
        }

        NkChrono::YieldThread();
    }

    int measuredFrames = 500 - 30;
    double avgFrameMs = totalFrameTimeMs / measuredFrames;
    double avgDrawMs = totalDrawTimeMs / measuredFrames;

    if (mode == "uniform") {
        logger.Infof("glUniform : %.2f ms par image, dessin %.2f ms", avgFrameMs, avgDrawMs);
    } else if (mode == "ubo") {
        logger.Infof("tampon d'uniformes : %.2f ms par image, dessin %.2f ms, alignement %d octets, pas %zu octets", avgFrameMs, avgDrawMs, alignment, taillePas);
    } else if (mode == "serre") {
        logger.Infof("tampon d'uniformes serre : %.2f ms par image, dessin %.2f ms, alignement %d octets, pas %zu octets", avgFrameMs, avgDrawMs, alignment, taillePas);
    }

    if (ubo != 0) glDeleteBuffers(1, &ubo);
    maillageUnique.Detruire();
    nuanceur.Detruire();
    contexte.Detruire();
    NkClose();
    return 0;
}