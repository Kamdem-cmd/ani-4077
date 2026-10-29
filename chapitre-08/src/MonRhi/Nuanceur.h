// =============================================================================
//  Étape 05 — chapitre 6 : un programme de nuanceurs, et ses journaux.
//
//  Un nuanceur (shader) est un petit programme qui tourne sur la carte
//  graphique. Il en faut DEUX pour dessiner quoi que ce soit :
//
//    - le nuanceur de SOMMETS s'exécute une fois par sommet et décide où le
//      point atterrit à l'écran ;
//    - le nuanceur de FRAGMENTS s'exécute une fois par pixel couvert et
//      décide de sa couleur.
//
//  Les deux sont écrits en GLSL, compilés à l'exécution par le pilote, puis
//  liés ensemble en un « programme ». C'est la seule partie d'OpenGL où vous
//  livrez du CODE SOURCE à votre utilisateur : le compilateur est dans son
//  pilote, pas chez vous.
//
//  CE QUE CETTE CLASSE APPORTE, ET QUI N'EST PAS DANS LES TUTORIELS
//  ================================================================
//  Elle LIT LES JOURNAUX. Un nuanceur qui ne compile pas ne fait pas planter
//  le programme : il rend un objet invalide, le dessin ne produit rien, et
//  l'écran reste noir sans un mot. Le message existe, il faut le demander.
//
//  Et elle règle le seul vrai obstacle à la portabilité du GLSL : la ligne
//  « #version », qui n'est PAS la même sur bureau et sur mobile. Voir
//  EnteteGlsl() dans le .cpp, c'est le passage à lire en premier.
// =============================================================================
#pragma once

#include "NKContainers/String/NkString.h"

namespace monrhi {

	// Un programme de nuanceurs : deux sources GLSL compilées et liées.
	//
	// La classe possède sa ressource OpenGL et la libère dans son
	// destructeur. Elle ne se copie pas : deux objets qui croiraient posséder
	// le même programme le supprimeraient deux fois.
	class Nuanceur {
		public:
			Nuanceur() = default;
			~Nuanceur();

			Nuanceur(const Nuanceur &) = delete;
			Nuanceur &operator=(const Nuanceur &) = delete;

			// Compile les deux sources et les lie.
			//
			// Les sources s'écrivent SANS ligne « #version » : Compiler la
			// pose lui-même, adaptée au système. Écrire « #version 330 core »
			// à la main dans votre source rendrait le nuanceur incompilable
			// sur mobile, et c'est l'erreur la plus fréquente du portage.
			//
			// Rend false et remplit Erreur() si quoi que ce soit échoue.
			bool Compiler(const nkentseu::NkString &sommets, const nkentseu::NkString &fragments);

			void Detruire();

			// L'active pour les dessins qui suivent. OpenGL est une machine à
			// états : ceci ne « passe » pas le programme au dessin, cela le
			// RANGE dans l'état global, et le prochain dessin s'en sert.
			// C'est le sujet entier du chapitre 7.
			void Activer() const;

			bool EstValide() const { return mProgramme != 0; }

			// Le journal du pilote, en clair. Vide si tout s'est bien passé.
			const nkentseu::NkString &Erreur() const { return mErreur; }

			// L'en-tête que Compiler a réellement posé. Le programme
			// l'imprime : c'est la preuve visible que le même code source
			// produit deux GLSL différents selon le système.
			static nkentseu::NkString EnteteGlsl(bool fragments);

			unsigned int Id() const { return mProgramme; }

		private:
			// Compile UN nuanceur et rend son identifiant, ou 0.
			unsigned int CompilerUn(unsigned int type, const nkentseu::NkString &source,
									const char *nomPourErreur);

			unsigned int mProgramme = 0;
			nkentseu::NkString mErreur;
	};

} // namespace monrhi
