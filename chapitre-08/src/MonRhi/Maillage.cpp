// =============================================================================
//  Étape 05 — le tampon, la recette, et l'ordre qui ne se discute pas.
// =============================================================================
#include "MonRhi/Maillage.h"
#include "MonRhi/Gl.h"

#include <cstddef>

namespace monrhi {

	Maillage::~Maillage() {
		Detruire();
	}

	bool Maillage::Creer(const Sommet *sommets, int nombre) {
		Detruire();
		if (sommets == nullptr || nombre <= 0)
			return false;

		// ── L'ORDRE COMPTE, ET IL N'EST PAS INTUITIF ────────────────────────
		// On lie le VAO EN PREMIER. Tout ce qui suit, y compris la liaison du
		// tampon et la description des attributs, est ENREGISTRÉ dans le VAO
		// courant. Lier le tampon avant le VAO fonctionne parfois, par
		// accident, et casse dès qu'un second maillage apparaît.
		glGenVertexArrays(1, &mVao);
		if (mVao == 0)
			return false;
		glBindVertexArray(mVao);

		glGenBuffers(1, &mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);

		// La COPIE vers la carte graphique. Après cette ligne, le tableau du
		// programme peut disparaître : OpenGL n'en a plus besoin.
		//
		// GL_STATIC_DRAW est un INDICE, pas une contrainte : « je téléverse
		// une fois et je dessine souvent ». Le pilote s'en sert pour choisir
		// où loger la mémoire. Se tromper ne casse rien, cela ralentit.
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sommet) * (size_t)nombre), sommets,
					 GL_STATIC_DRAW);

		// ── LA RECETTE, ATTRIBUT PAR ATTRIBUT ───────────────────────────────
		// location 0 : la position, trois flottants, au début de la structure.
		// Le « 0 » correspond au « layout(location = 0) » du nuanceur de
		// sommets. Ces deux zéros DOIVENT correspondre, et rien ne le vérifie
		// pour vous : se tromper donne un triangle noir, ou pas de triangle.
		//
		// Le cinquième argument, le PAS, vaut sizeof(Sommet) et non 3*4 : il
		// dit de combien avancer d'un sommet au suivant, couleur comprise.
		// C'est l'erreur classique, et elle donne un triangle déformé.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Sommet),
							  (const void *)offsetof(Sommet, x));
		glEnableVertexAttribArray(0);

		// location 1 : la couleur, trois flottants, décalée de douze octets.
		// offsetof plutôt qu'un 12 écrit à la main : le jour où un champ
		// s'ajoute avant, le 12 devient faux en silence.
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Sommet),
							  (const void *)offsetof(Sommet, r));
		glEnableVertexAttribArray(1);

		// On délie, pour que la suite du programme ne modifie pas ce VAO par
		// accident. Ce n'est pas obligatoire, c'est de l'hygiène, et le
		// chapitre 7 explique pourquoi elle finit par sauver une soirée.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		mNombre = nombre;
		return true;
	}

	void Maillage::Detruire() {
		if (mVbo != 0) {
			glDeleteBuffers(1, &mVbo);
			mVbo = 0;
		}
		if (mVao != 0) {
			glDeleteVertexArrays(1, &mVao);
			mVao = 0;
		}
		mNombre = 0;
	}

	void Maillage::Dessiner() const {
		if (mVao == 0)
			return;
		glBindVertexArray(mVao);
		// L'APPEL DE DESSIN. Trois arguments : quelle forme, à partir de quel
		// sommet, combien de sommets. Il ne dit ni quoi dessiner, ni avec quel
		// nuanceur : tout cela est dans l'état, posé avant.
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mNombre);
		glBindVertexArray(0);
	}

} // namespace monrhi
