#include "MonRhi/MonRhi.h"

#include "NKWindow/Core/NkSurface.h"
#include "NKContainers/String/NkFormat.h"

namespace monrhi {

	const char *Version() { return "MonRhi 0.5"; }

	nkentseu::NkString BackendActif() { return "aucun"; }

	// ── POURQUOI PAS snprintf, ALORS QUE C'EST CE QUE TOUT LE MONDE ÉCRIT ─────
	//
	// La version au tampon fixe de ce livre disait :
	//
	//     char tampon[512];
	//     int n = std::snprintf(tampon, sizeof(tampon), "...", ...);
	//     std::snprintf(tampon + n, sizeof(tampon) - n, "...", ...);
	//
	// Elle a trois défauts, et le troisième est un vrai bogue :
	//
	//   1. 512 est un nombre inventé. Personne ne sait s'il suffit.
	//   2. Si la première écriture tronque, `n` vaut ce qu'il AURAIT fallu, pas
	//      ce qui a été écrit. `tampon + n` sort alors du tampon, et
	//      `sizeof(tampon) - n` devient un très grand nombre non signé. C'est
	//      dans le standard, et presque personne ne le sait.
	//   3. Nkentseu a déjà la réponse, et la chercher avant d'écrire `std::`
	//      est la règle de ce livre.
	//
	// NkPrintf rend une NkString qui grandit toute seule. Il n'y a plus de
	// taille à deviner, donc plus de troncature, donc plus de calcul faux.
	nkentseu::NkString DecrireSurface(const nkentseu::NkSurfaceDesc &s) {
		nkentseu::NkString texte = nkentseu::NkPrintf(
			"surface %ux%u, dpi %.2f, valide : %s",
			(unsigned)s.width, (unsigned)s.height,
			(double)s.dpi, s.IsValid() ? "oui" : "non");

#if defined(NKENTSEU_PLATFORM_WINDOWS)
		// NkPrintfTo ajoute à la suite, sans recalculer d'offset et sans
		// jamais pouvoir se tromper de taille restante.
		nkentseu::NkPrintfTo(texte, "\n  hwnd      : %p\n  hinstance : %p",
							 (void *)s.hwnd, (void *)s.hinstance);
#endif
		return texte;
	}

} // namespace monrhi
