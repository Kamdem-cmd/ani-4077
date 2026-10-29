// =============================================================================
//  Étape 05 — chapitre 6 : le premier triangle.
//
//  Le chapitre 5 s'est arrêté sur un rectangle pétrole vide. Celui-ci met
//  quelque chose dedans, et c'est la première fois du livre que la carte
//  graphique CALCULE au lieu d'effacer.
//
//  Tout ce que faisait l'étape 04 continue de fonctionner : la fenêtre, le
//  contexte écrit à la main, l'effacement en pétrole, et la surface qui va et
//  vient sur mobile. On n'a rien remplacé, on a ajouté.
//
//  MONRHI_IMAGES=n  arrête au bout de n images (pour les captures du livre).
//  MONRHI_TAIRE=1   n'imprime que l'essentiel.
// =============================================================================
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

	// ── LE NUANCEUR DE SOMMETS ───────────────────────────────────────────────
	// Il s'exécute UNE FOIS PAR SOMMET, donc trois fois pour un triangle.
	//
	// Remarquez ce qui n'y est pas : aucune ligne « #version ». Elle est posée
	// par Nuanceur::EnteteGlsl selon le système, et c'est ce qui permet à ces
	// quinze lignes d'être les mêmes sur Windows, Linux, Android, HarmonyOS et
	// le Web.
	//
	// gl_Position est la SEULE sortie obligatoire. Elle a quatre composantes,
	// pas trois : la quatrième, w, vaut 1 pour l'instant et deviendra le
	// sujet du chapitre 16, quand la perspective arrivera.
	const char *kSommets = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aCouleur;

out vec3 vCouleur;

void main() {
    vCouleur = aCouleur;
    gl_Position = vec4(aPosition, 1.0);
}
)";

	// ── LE NUANCEUR DE FRAGMENTS ─────────────────────────────────────────────
	// Il s'exécute UNE FOIS PAR PIXEL COUVERT. Pour un triangle qui occupe la
	// moitié d'une fenêtre 1280x720, cela fait plus de 460 000 exécutions par
	// image, à soixante images par seconde. C'est pour cela qu'on ne met pas
	// de « if » compliqués ici.
	//
	// vCouleur n'est PAS la couleur d'un sommet : c'est la couleur INTERPOLÉE
	// entre les trois, calculée par la carte pour ce pixel précis. On n'a
	// donné que trois couleurs, on en obtient des centaines de milliers.
	const char *kFragments = R"(
in vec3 vCouleur;

out vec4 oCouleur;

void main() {
    oCouleur = vec4(vCouleur, 1.0);
}
)";

	// Le triangle, en coordonnées d'écran normalisées : -1 à gauche, +1 à
	// droite, -1 en bas, +1 en haut, quelle que soit la taille de la fenêtre.
	// Aucune matrice, aucune caméra : c'est le chapitre 16 qui les apportera.
	const monrhi::Sommet kTriangle[3] = {
		{  0.0f,  0.6f, 0.0f,   0.96f, 0.62f, 0.14f },   // haut, or
		{ -0.7f, -0.5f, 0.0f,   0.20f, 0.75f, 0.70f },   // bas gauche, turquoise
		{  0.7f, -0.5f, 0.0f,   0.90f, 0.30f, 0.35f },   // bas droite, rouge
	};

} // namespace

int nkmain(const NkEntryState &state) {
	(void)state;

	NkInitialise();

	// LA MINUTERIE FINE, ET CE N'EST PAS UN DETAIL.
	// Sans cet appel, un sommeil de 16 ms dure 29,76 ms sur Windows :
	// mesure du moteur, 15 aout 2026, 40 repetitions. La boucle ne tourne
	// alors pas a 60 images par seconde mais a 33, et rien ne le dit.
	NkChrono::BeginPreciseTiming();

	NkWindowConfig cfg;
	cfg.title = "Etape 05 - le premier triangle";
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

	// ── CE QUE L'ÉTAPE 04 NE FAISAIT PAS ─────────────────────────────────────
	// L'en-tête GLSL réellement posé s'imprime : c'est la preuve visible que
	// les mêmes quinze lignes de source produisent deux GLSL différents.
	logger.Infof("glsl    : %s",
				monrhi::Nuanceur::EnteteGlsl(false).CStr());

	monrhi::Nuanceur nuanceur;
	if (!nuanceur.Compiler(kSommets, kFragments)) {
		// ON IMPRIME LE JOURNAL. Sans ces trois lignes, un nuanceur qui ne
		// compile pas donne un ecran vide et un programme qui a l'air d'aller
		// tres bien. Le message du pilote existe, il faut le demander.
		logger.Errorf("NUANCEUR REFUSE :\n%s", nuanceur.Erreur().CStr());
		contexte.Detruire();
		NkClose();
		return 1;
	}
	logger.Infof("nuanceur: programme %u, compile et lie", nuanceur.Id());

	monrhi::Maillage triangle;
	if (!triangle.Creer(kTriangle, 3)) {
		logger.Error("le maillage n'a pas pu etre cree");
		contexte.Detruire();
		NkClose();
		return 1;
	}
	logger.Infof("maillage: tampon %u, tableau %u, %d sommets, %zu octets",
				triangle.IdTampon(), triangle.IdTableau(), triangle.Nombre(),
				sizeof(kTriangle));

	// Meme l'environnement passe par le moteur : NkEnv enveloppe
	// GetEnvironmentVariable sous Windows et getenv ailleurs.
	const char *limite = nkentseu::env::GetEnvVar("MONRHI_IMAGES");
	const unsigned long long maxImages = limite != nullptr ? std::strtoull(limite, nullptr, 10) : 0;

	unsigned long long images = 0;
	bool tourne = true;

	while (tourne && fenetre.IsOpen()) {
		NkEvent *ev = nullptr;
		while (NkEvents().PollEvent(ev)) {
			if (ev->As<NkWindowCloseEvent>() != nullptr)
				tourne = false;

			// La surface qui va et vient sur mobile, comme au chapitre 5. On
			// écoute les QUATRE événements : Android et HarmonyOS ne disent
			// pas la même chose avec les mêmes mots.
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

		// TROIS LIGNES, ET C'EST TOUT LE CHAPITRE.
		// On RANGE le programme dans l'état, puis on dessine. Le dessin ne
		// reçoit pas le nuanceur en argument : il prend ce qui est là.
		nuanceur.Activer();
		triangle.Dessiner();

		contexte.Echanger();

		// ON REND LA MAIN. Sur le Web, cette ligne est la difference entre
		// une demo qui tourne et un onglet gele : YieldThread appelle
		// emscripten_sleep(0), qui laisse le navigateur dessiner et
		// reprendre ensuite ici meme. Ailleurs, elle cede le quantum au
		// systeme et ne coute rien.
		NkChrono::YieldThread();

		++images;
		if (maxImages != 0 && images >= maxImages)
			break;
	}

	logger.Infof("%llu images, fermeture propre", images);

	// L'ordre inverse de la création, toujours. Le maillage et le nuanceur
	// vivent DANS le contexte : ils partent avant lui.
	triangle.Detruire();
	nuanceur.Detruire();
	contexte.Detruire();
	NkChrono::EndPreciseTiming();
	NkClose();
	return 0;
}
