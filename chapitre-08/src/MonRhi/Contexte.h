// =============================================================================
//  Étape 04 — chapitre 5 : le contexte OpenGL, écrit à la main.
//
//  Ce fichier est le premier de MonRhi qui a le droit de dire « OpenGL ». Et
//  pourtant, regardez-le : il ne contient pas une seule fonction gl*, pas un
//  seul type de la plateforme, et aucun #if.
//
//  Ce n'est pas de la coquetterie, c'est la contrainte qui décide de tout le
//  reste du livre :
//
//    UN EN-TÊTE PUBLIC N'INCLUT PAS <windows.h>.
//
//  Si Contexte.h incluait <windows.h>, alors CHAQUE fichier de votre moteur
//  qui parle de rendu hériterait de deux cent mille lignes d'en-têtes, et
//  surtout des macros near, far, min, max et CreateWindow, qui cassent le
//  code de mathématiques dès qu'on écrit `float near = 0.1f;`.
//
//  La technique employée s'appelle le POINTEUR VERS L'IMPLÉMENTATION. On
//  déclare une structure sans la définir, on n'en garde qu'un pointeur, et
//  toute la vérité de la plateforme vit dans Contexte.cpp. Vous la
//  retrouverez au chapitre 12, sous le nom de « poignée opaque », appliquée
//  cette fois aux tampons et aux textures.
// =============================================================================
#pragma once

#include "NKContainers/String/NkString.h"

namespace nkentseu {
	struct NkSurfaceDesc;
}

namespace monrhi {

	// Le chemin réellement emprunté pour obtenir le contexte. Cinq systèmes,
	// quatre chemins : Android et HarmonyOS partagent EGL, et c'est la seule
	// bonne nouvelle de ce chapitre.
	enum class CheminGL {
		Aucun,  // rien n'a été créé
		Wgl,    // Windows
		Glx,    // Linux sous X11
		Egl,    // Android, HarmonyOS
		WebGL   // navigateur, par Emscripten
	};

	const char *NomDuChemin(CheminGL chemin);

	class Contexte {
		public:
			Contexte() = default;
			~Contexte();

			// Ni copie ni affectation : un contexte est une ressource du
			// pilote, pas une valeur. Copier l'objet copierait la poignée, et
			// le second destructeur détruirait un contexte déjà détruit.
			// C'est la première fois du livre qu'on l'écrit ; ce ne sera pas
			// la dernière.
			Contexte(const Contexte &) = delete;
			Contexte &operator=(const Contexte &) = delete;

			// LE point d'entrée. On ne lui donne QUE le descripteur de
			// surface : pas la fenêtre, pas le moteur, pas de configuration.
			// Tout ce dont il a besoin tient dans une poignée native.
			bool Creer(const nkentseu::NkSurfaceDesc &surface);

			void Detruire();

			// Présente ce qui a été dessiné. Le nom du livre est « échanger »
			// parce que c'est ce qui se passe : le tampon caché devient
			// visible et l'ancien visible devient le prochain caché.
			void Echanger();

			// Redevenir le contexte courant du fil d'exécution. Inutile tant
			// qu'il n'y a qu'un contexte, indispensable au chapitre 21.
			void RendreCourant();

			// ── LA SURFACE PEUT DISPARAÎTRE SOUS VOS PIEDS ──────────────────
			// Sur mobile, quand l'utilisateur passe à une autre application, le
			// système DÉTRUIT la surface de rendu. Le contexte, lui, survit
			// avec toutes ses ressources. Au retour, une surface NEUVE est
			// créée, et il faut s'y rattacher.
			//
			// Sans ces deux appels, le programme continue d'échanger dans une
			// surface morte : l'écran reste noir, et le pilote répète
			// « EGL_BAD_SURFACE » des centaines de fois par seconde sans que
			// rien ne s'arrête. C'est le défaut le plus signalé des premières
			// applications mobiles.
			//
			// Sur Windows, Linux et le Web, ces deux méthodes ne font RIEN :
			// la surface d'une fenêtre de bureau ne disparaît pas. L'interface
			// reste la même partout, et c'est précisément ce qu'on attend
			// d'une couche d'abstraction.
			void DetacherSurface();
			bool RattacherSurface(const nkentseu::NkSurfaceDesc &surface);

			// Faux entre la destruction et la recréation de la surface. Tant
			// qu'il rend faux, ne dessinez pas : il n'y a nulle part où le
			// faire.
			bool PeutDessiner() const;

			bool EstCree() const { return mInterne != nullptr; }

			CheminGL Chemin() const { return mChemin; }

			// Ce que le pilote raconte de lui-même. Vide tant que le contexte
			// n'existe pas, et c'est volontaire : glGetString sans contexte
			// courant ne rend pas une chaîne vide, il plante.
			nkentseu::NkString VersionGL() const;
			nkentseu::NkString Carte() const;

			// Une ligne prête à imprimer, qui sera reprise telle quelle par
			// tous les programmes du livre.
			nkentseu::NkString Decrire() const;

		private:
			// Déclarée ici, définie NULLE PART dans cet en-tête. C'est tout
			// le procédé.
			struct Interne;
			Interne *mInterne = nullptr;
			CheminGL mChemin = CheminGL::Aucun;
	};

} // namespace monrhi
