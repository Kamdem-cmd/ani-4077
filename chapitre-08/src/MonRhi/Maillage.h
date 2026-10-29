// =============================================================================
//  Étape 05 — chapitre 6 : des sommets, un tampon, et la recette pour les lire.
//
//  TROIS OBJETS, ET ON LES CONFOND TOUT LE TEMPS
//  =============================================
//  Pour dessiner un triangle il faut trois choses distinctes, et la moitié des
//  premiers bogues vient de croire que c'en est une seule.
//
//    1. VOS DONNÉES, dans la mémoire de votre programme. Un tableau de
//       flottants. OpenGL ne les voit pas.
//
//    2. LE TAMPON (VBO, Vertex Buffer Object). Un bloc de mémoire du côté de
//       la carte graphique. On y COPIE vos données. À partir de là, modifier
//       votre tableau ne change plus rien à l'écran.
//
//    3. L'OBJET DE TABLEAU DE SOMMETS (VAO, Vertex Array Object). Il ne
//       contient AUCUNE donnée. C'est une RECETTE : « la position est faite de
//       trois flottants, elle commence à l'octet 0, et chaque sommet occupe
//       24 octets ». Sans lui, le tampon n'est qu'une suite d'octets dont
//       personne ne sait quoi faire.
//
//  L'image qui a fonctionné avec la promotion : le tampon est le carton de
//  déménagement, le VAO est l'étiquette qui dit ce qu'il y a dedans et dans
//  quel ordre. Un carton sans étiquette est inutilisable ; une étiquette sans
//  carton ne décrit rien.
// =============================================================================
#pragma once

namespace monrhi {

	// Un sommet de ce chapitre : une position et une couleur.
	//
	// La couleur EST une donnée de sommet, exactement comme la position. C'est
	// ce qui produit le dégradé du triangle : on ne donne que trois couleurs,
	// une par coin, et la carte interpole tout le reste. Ce mot,
	// « interpole », est le sujet du chapitre 7.
	struct Sommet {
		float x, y, z;
		float r, v, b;
	};

	// Un maillage : un tampon de sommets et sa recette de lecture.
	//
	// Il possède ses ressources OpenGL et ne se copie pas, pour la même raison
	// que Nuanceur : deux objets qui croiraient posséder le même tampon le
	// supprimeraient deux fois.
	class Maillage {
		public:
			Maillage() = default;
			~Maillage();

			Maillage(const Maillage &) = delete;
			Maillage &operator=(const Maillage &) = delete;

			// Téléverse les sommets et décrit leur disposition.
			// Rend false s'il n'y a pas de contexte courant.
			bool Creer(const Sommet *sommets, int nombre);

			void Detruire();

			// Dessine. Le nuanceur doit avoir été activé AVANT : OpenGL prend
			// ce qui est dans son état, il ne demande rien.
			void Dessiner() const;

			bool EstValide() const { return mVao != 0 && mVbo != 0; }

			int Nombre() const { return mNombre; }

			// Les identifiants bruts, pour que le livre puisse les imprimer.
			// Un vrai code n'en aurait pas besoin.
			unsigned int IdTampon() const { return mVbo; }
			unsigned int IdTableau() const { return mVao; }

		private:
			unsigned int mVao = 0;
			unsigned int mVbo = 0;
			int mNombre = 0;
	};

} // namespace monrhi
