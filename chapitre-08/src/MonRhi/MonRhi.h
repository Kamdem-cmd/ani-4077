// =============================================================================
//  Étape 03 — MonRhi apprend à compter des fronts.
//
//  Un front, c'est le passage de « relâchée » à « pressée ». L'état, lui, dit
//  seulement « pressée » ou « non ». Les deux sont utiles, et confondre les
//  deux est la moitié des bogues d'interaction.
//
//  Cette classe est le premier morceau de MonRhi qui a de la MÉMOIRE : elle
//  compare l'image en cours à la précédente. Tout le reste du livre reposera
//  sur cette idée, jusqu'aux poignées du chapitre 9.
// =============================================================================
#pragma once

#include "NKContainers/String/NkString.h"

namespace nkentseu {
	struct NkSurfaceDesc;
	struct NkEventState;
}

namespace monrhi {

	const char *Version();
	nkentseu::NkString BackendActif();
	nkentseu::NkString DecrireSurface(const nkentseu::NkSurfaceDesc &surface);

	// Le détecteur de fronts. Il ne connaît ni le clavier ni la souris : on
	// lui donne un booléen par image, il dit s'il vient de changer.
	class Front {
		public:
			// Rend true UNE SEULE FOIS, à l'image où l'état passe à true.
			bool Monte(bool etatCourant) {
				const bool front = etatCourant && !mPrecedent;
				mPrecedent = etatCourant;
				return front;
			}

			bool Descend(bool etatCourant) {
				const bool front = !etatCourant && mPrecedent;
				mPrecedent = etatCourant;
				return front;
			}

			bool Precedent() const { return mPrecedent; }

		private:
			// L'état de l'image précédente. C'est TOUT ce qu'il faut de
			// mémoire, et beaucoup de gens en gardent bien davantage.
			bool mPrecedent = false;
	};

} // namespace monrhi
