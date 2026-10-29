// =============================================================================
//  Étape 04 — les cinq temps du contexte, sur cinq systèmes.
//
//  Ce fichier est LE fichier du chapitre 5. Il est long, il est plein de
//  #if, et c'est exactement le propos : voici ce que coûte, en lignes
//  réelles, le droit de dessiner un pixel sur cinq systèmes.
//
//  Il est aussi le seul fichier de MonRhi qui aura le droit d'être laid.
//  Tout ce qui suit dans le livre sera propre PARCE QUE celui-ci absorbe la
//  saleté. C'est la définition même d'une couche d'abstraction, et vous
//  venez d'en écrire une avant qu'on vous en donne le nom.
//
//  Les cinq temps, dans l'ordre, et ils sont les mêmes partout :
//
//    1. obtenir de quoi parler à la surface  (HDC / Display / EGLDisplay)
//    2. choisir un format de pixel           (PFD / FBConfig / EGLConfig)
//    3. créer un contexte                    (et parfois deux, cf. Windows)
//    4. le rendre courant                    (MakeCurrent, partout)
//    5. charger les fonctions d'OpenGL       (glad)
//
//  Retenez cette liste : elle est la carte de tout ce fichier, et elle sera
//  encore vraie au tome Vulkan, où seuls les noms changent.
// =============================================================================
#include "MonRhi/Contexte.h"
#include "NKLogger/NkLog.h"
#include "NKContainers/String/NkString.h"

#include "NKWindow/Core/NkSurface.h"

#include <cstring>

// -----------------------------------------------------------------------------
//  Gl.h choisit entre OpenGL et OpenGL ES. Ici, on ajoute par-dessus ce qui
//  sert à FABRIQUER le contexte, et qui n'a rien à voir avec le dessin :
//  wgl, glx, egl. Ces trois-là ne sont inclus nulle part ailleurs.
// -----------------------------------------------------------------------------
#include "MonRhi/Gl.h"

#if defined(NKENTSEU_PLATFORM_WINDOWS)
#include <glad/wgl.h>

#elif defined(NKENTSEU_WINDOWING_XLIB)
#include <glad/glx.h>

#elif defined(NKENTSEU_PLATFORM_ANDROID) || defined(NKENTSEU_PLATFORM_HARMONYOS)
#include <glad/egl.h>
#include <unistd.h> // usleep, pour les réessais d'EGL

#elif defined(NKENTSEU_PLATFORM_EMSCRIPTEN)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>
#endif

namespace monrhi {

	const char *NomDuChemin(CheminGL chemin) {
		switch (chemin) {
			case CheminGL::Wgl:
				return "WGL (Windows)";
			case CheminGL::Glx:
				return "GLX (Linux/X11)";
			case CheminGL::Egl:
				return "EGL (Android, HarmonyOS)";
			case CheminGL::WebGL:
				return "WebGL (navigateur)";
			default:
				return "aucun";
		}
	}

	// -------------------------------------------------------------------------
	//  Interne : tout ce que l'en-tête refuse de savoir.
	//
	//  Un champ par système, et ils ne coexistent jamais : le préprocesseur
	//  n'en garde qu'un jeu. La structure fait donc vingt-quatre octets sur
	//  Windows et trente-deux sur Android, et personne au-dehors ne le sait.
	// -------------------------------------------------------------------------
	struct Contexte::Interne {
#if defined(NKENTSEU_PLATFORM_WINDOWS)
		HWND fenetre = nullptr;
		HDC dc = nullptr;
		HGLRC rc = nullptr;

#elif defined(NKENTSEU_WINDOWING_XLIB)
		Display *affichage = nullptr;
		::Window fenetre = 0;
		GLXContext rc = nullptr;

#elif defined(NKENTSEU_PLATFORM_ANDROID) || defined(NKENTSEU_PLATFORM_HARMONYOS)
		EGLDisplay affichage = EGL_NO_DISPLAY;
		EGLSurface surface = EGL_NO_SURFACE;
		EGLContext rc = EGL_NO_CONTEXT;
		EGLConfig config = nullptr;

#elif defined(NKENTSEU_PLATFORM_EMSCRIPTEN)
		EMSCRIPTEN_WEBGL_CONTEXT_HANDLE rc = 0;

#else
		int rien = 0;
#endif
	};

	Contexte::~Contexte() { Detruire(); }

	// =========================================================================
	//  WINDOWS — la danse en cinq temps de WGL
	// =========================================================================
#if defined(NKENTSEU_PLATFORM_WINDOWS)

	bool Contexte::Creer(const nkentseu::NkSurfaceDesc &surface) {
		if (mInterne != nullptr) {
			logger.Info("[contexte] deja cree");
			return false;
		}
		if (surface.hwnd == nullptr) {
			logger.Error("[contexte] hwnd absent : la fenetre ne s'est pas ouverte");
			return false;
		}

		Interne *ctx = new Interne();
		ctx->fenetre = (HWND)surface.hwnd;

		// --- Temps 1 : le contexte de PÉRIPHÉRIQUE ---------------------------
		// Ne pas confondre avec le contexte OpenGL, qui viendra au temps 3.
		// Celui-ci est une notion de Windows, vieille de 1985, et il désigne
		// « la surface sur laquelle GDI a le droit de peindre ».
		//
		// Il n'est PAS dans le descripteur de surface, et c'est délibéré :
		// NkSurface.h le dit en toutes lettres, un HDC est éphémère. On le
		// demande au moment de s'en servir, on ne le range pas.
		ctx->dc = GetDC(ctx->fenetre);
		if (ctx->dc == nullptr) {
			logger.Error("[contexte] GetDC a echoue");
			delete ctx;
			return false;
		}

		// --- Temps 2 : le format de pixel ------------------------------------
		// On décrit le tampon qu'on veut : combien de bits de couleur, de
		// profondeur, de pochoir, et surtout DOUBLE TAMPON.
		//
		// Cette structure date de 1992. Elle contient des champs pour les
		// palettes de couleurs indexées et pour les « couches de recouvrement
		// matérielles », deux choses que plus aucune carte ne fait. On les
		// remplit quand même, parce que l'appel les lit.
		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		// ChoosePixelFormat ne rend PAS le format demandé : il rend le plus
		// proche que la carte sait faire. Demander 32 bits de profondeur sur
		// une carte qui n'en offre que 24 ne produit aucune erreur, on
		// obtient 24. Pour savoir ce qu'on a réellement, il faut relire par
		// DescribePixelFormat, et c'est l'exercice 5.
		const int format = ChoosePixelFormat(ctx->dc, &pfd);
		if (format == 0 || SetPixelFormat(ctx->dc, format, &pfd) == FALSE) {
			logger.Error("[contexte] format de pixel refuse");
			ReleaseDC(ctx->fenetre, ctx->dc);
			delete ctx;
			return false;
		}

		// --- Temps 3a : un contexte dont on ne veut pas ------------------------
		// Voici la bizarrerie que ce chapitre existe pour expliquer.
		//
		// wglCreateContext rend un contexte ANCIEN : profil de compatibilité,
		// version décidée par le pilote, sans aucun moyen de demander autre
		// chose. Pour demander un contexte moderne il faut appeler
		// wglCreateContextAttribsARB.
		//
		// Or wglCreateContextAttribsARB est une EXTENSION. On l'obtient par
		// wglGetProcAddress. Et wglGetProcAddress ne fonctionne que s'il y a
		// déjà un contexte courant.
		//
		// Donc : IL FAUT UN CONTEXTE POUR DEMANDER UN CONTEXTE. On en crée un
		// dont le seul rôle est d'être jeté quinze lignes plus bas.
		HGLRC provisoire = wglCreateContext(ctx->dc);
		if (provisoire == nullptr) {
			logger.Error("[contexte] wglCreateContext (provisoire) a echoue");
			ReleaseDC(ctx->fenetre, ctx->dc);
			delete ctx;
			return false;
		}
		wglMakeCurrent(ctx->dc, provisoire);

		// gladLoaderLoadWGL fait le wglGetProcAddress pour nous, sur toutes
		// les fonctions WGL d'un coup. C'est ici qu'il DOIT être appelé : ni
		// avant (pas de contexte courant), ni après (on aura besoin de
		// wglCreateContextAttribsARB à la ligne suivante).
		if (gladLoaderLoadWGL(ctx->dc) == 0) {
			logger.Error("[contexte] chargement des extensions WGL impossible");
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(provisoire);
			ReleaseDC(ctx->fenetre, ctx->dc);
			delete ctx;
			return false;
		}

		// --- Temps 3b : le vrai contexte -------------------------------------
		// 3.3 core, et pas davantage. Ce livre pourrait demander 4.6 : il ne
		// le fait pas, et la raison est au chapitre 1. Tout ce qu'on écrira
		// tient en 3.3, 3.3 tourne sur des machines de 2010, et vos étudiants
		// n'ont pas tous une carte de 2024.
		//
		// « core » veut dire : les fonctions supprimées sont VRAIMENT
		// supprimées. glBegin n'existe pas, la matrice fixe n'existe pas, et
		// tout tutoriel trouvé en ligne qui commence par glBegin ne compilera
		// pas. C'est une bonne nouvelle : il date de 2004.
		const int attributs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
								 3,
								 WGL_CONTEXT_MINOR_VERSION_ARB,
								 3,
								 WGL_CONTEXT_PROFILE_MASK_ARB,
								 WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
								 0};

		ctx->rc = wglCreateContextAttribsARB(ctx->dc, nullptr, attributs);
		if (ctx->rc == nullptr) {
			logger.Error("[contexte] wglCreateContextAttribsARB a echoue (3.3 core refuse)");
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(provisoire);
			ReleaseDC(ctx->fenetre, ctx->dc);
			delete ctx;
			return false;
		}

		// --- Temps 4 : rendre courant, et jeter le provisoire -----------------
		// L'ordre compte. On DÉTACHE avant de détruire : supprimer un
		// contexte courant est un comportement indéfini, et sur certains
		// pilotes cela « marche », ce qui est pire.
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(provisoire);
		if (wglMakeCurrent(ctx->dc, ctx->rc) == FALSE) {
			logger.Error("[contexte] wglMakeCurrent sur le contexte 3.3 a echoue");
			wglDeleteContext(ctx->rc);
			ReleaseDC(ctx->fenetre, ctx->dc);
			delete ctx;
			return false;
		}

		// --- Temps 5 : charger les fonctions d'OpenGL -------------------------
		// Sur Windows, opengl32.dll n'exporte qu'OpenGL 1.1. Absolument tout
		// ce qui est arrivé depuis 1997 doit être demandé au pilote un
		// pointeur à la fois. glad le fait pour les mille et quelques
		// fonctions d'un coup.
		//
		// Sans cette ligne, glCreateShader n'est pas « introuvable » : elle
		// est NULLE, et l'appeler saute à l'adresse zéro. Le message est
		// « Access violation at 0x0000000000000000 », et il ne nomme pas
		// OpenGL. C'est le plantage le plus fréquent d'un premier programme.
		if (gladLoaderLoadGL() == 0) {
			logger.Error("[contexte] gladLoaderLoadGL a echoue");
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(ctx->rc);
			ReleaseDC(ctx->fenetre, ctx->dc);
			delete ctx;
			return false;
		}

		mInterne = ctx;
		mChemin = CheminGL::Wgl;
		return true;
	}

	void Contexte::Detruire() {
		if (mInterne == nullptr)
			return;
		wglMakeCurrent(nullptr, nullptr);
		if (mInterne->rc != nullptr)
			wglDeleteContext(mInterne->rc);
		if (mInterne->dc != nullptr)
			ReleaseDC(mInterne->fenetre, mInterne->dc);
		delete mInterne;
		mInterne = nullptr;
		mChemin = CheminGL::Aucun;
	}

	void Contexte::Echanger() {
		if (mInterne != nullptr)
			SwapBuffers(mInterne->dc);
	}

	void Contexte::RendreCourant() {
		if (mInterne != nullptr)
			wglMakeCurrent(mInterne->dc, mInterne->rc);
	}

	void Contexte::DetacherSurface() {
		// Rien. Une fenetre Windows ne perd pas sa surface : le HDC reste
		// valide tant que la fenetre existe, et si elle n'existe plus, le
		// programme a d'autres soucis.
	}

	bool Contexte::RattacherSurface(const nkentseu::NkSurfaceDesc &surface) {
		(void)surface;
		return mInterne != nullptr;
	}

	bool Contexte::PeutDessiner() const {
		return mInterne != nullptr;
	}

	// =========================================================================
	//  LINUX SOUS X11 — GLX, la même danse avec d'autres noms
	// =========================================================================
#elif defined(NKENTSEU_WINDOWING_XLIB)

	bool Contexte::Creer(const nkentseu::NkSurfaceDesc &surface) {
		if (mInterne != nullptr)
			return false;
		if (surface.display == nullptr || surface.window == 0) {
			logger.Error("[contexte] Display ou Window absent");
			return false;
		}

		Interne *ctx = new Interne();
		ctx->affichage = surface.display;
		ctx->fenetre = surface.window;
		const int ecran = surface.screen;

		// --- Temps 1 et 5, mélangés : le problème de l'œuf ---------------------
		// Sous Windows, on avait besoin d'un contexte pour charger WGL. Sous
		// Linux, c'est plus doux : glad sait ouvrir libGL.so tout seul, par
		// dlopen, et en tirer les fonctions GLX AVANT tout contexte.
		//
		// Mais il FAUT le faire, et c'est le piège symétrique : sans cette
		// ligne, glXChooseFBConfig est un pointeur nul, et le programme
		// meurt sur un SIGSEGV à l'adresse zéro, exactement comme sous
		// Windows sans gladLoaderLoadGL.
		if (gladLoaderLoadGLX(ctx->affichage, ecran) == 0) {
			logger.Error("[contexte] gladLoaderLoadGLX a echoue (libGL introuvable ?)");
			delete ctx;
			return false;
		}

		// --- Temps 2 : le format de pixel, qui s'appelle ici FBConfig ---------
		// Même idée que le PIXELFORMATDESCRIPTOR, forme différente : une
		// liste de paires clé-valeur terminée par un zéro. C'est la
		// convention de toute la famille X, et vous la retrouverez telle
		// quelle dans EGL et dans les attributs de contexte.
		const int attributsFb[] = {GLX_X_RENDERABLE, True,      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
								   GLX_RENDER_TYPE,  GLX_RGBA_BIT, GLX_RED_SIZE,   8,
								   GLX_GREEN_SIZE,   8,         GLX_BLUE_SIZE,     8,
								   GLX_ALPHA_SIZE,   8,         GLX_DEPTH_SIZE,    24,
								   GLX_STENCIL_SIZE, 8,         GLX_DOUBLEBUFFER,  True,
								   0};

		int nombre = 0;
		GLXFBConfig *configs = glXChooseFBConfig(ctx->affichage, ecran, attributsFb, &nombre);
		if (configs == nullptr || nombre <= 0) {
			logger.Info("[contexte] glXChooseFBConfig : aucune configuration compatible");
			delete ctx;
			return false;
		}
		GLXFBConfig config = configs[0];
		XFree(configs);

		// --- Temps 3 : le contexte 3.3 core ----------------------------------
		// Pas de contexte provisoire ici : glad a déjà chargé
		// glXCreateContextAttribsARB au temps 1. C'est la seule différence
		// de fond entre WGL et GLX, et elle explique pourquoi la version
		// Windows de cette fonction est cinquante lignes plus longue.
		const int attributsCtx[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,
									3,
									GLX_CONTEXT_MINOR_VERSION_ARB,
									3,
									GLX_CONTEXT_PROFILE_MASK_ARB,
									GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
									0};

		ctx->rc = glXCreateContextAttribsARB(ctx->affichage, config, nullptr, True, attributsCtx);
		if (ctx->rc == nullptr) {
			logger.Error("[contexte] glXCreateContextAttribsARB a echoue");
			delete ctx;
			return false;
		}

		// --- Temps 4 : rendre courant ----------------------------------------
		if (glXMakeCurrent(ctx->affichage, ctx->fenetre, ctx->rc) == False) {
			logger.Error("[contexte] glXMakeCurrent a echoue");
			glXDestroyContext(ctx->affichage, ctx->rc);
			delete ctx;
			return false;
		}

		// --- Temps 5 : les fonctions d'OpenGL --------------------------------
		if (gladLoaderLoadGL() == 0) {
			logger.Error("[contexte] gladLoaderLoadGL a echoue");
			glXMakeCurrent(ctx->affichage, 0, nullptr);
			glXDestroyContext(ctx->affichage, ctx->rc);
			delete ctx;
			return false;
		}

		mInterne = ctx;
		mChemin = CheminGL::Glx;
		return true;
	}

	void Contexte::Detruire() {
		if (mInterne == nullptr)
			return;
		glXMakeCurrent(mInterne->affichage, 0, nullptr);
		if (mInterne->rc != nullptr)
			glXDestroyContext(mInterne->affichage, mInterne->rc);
		delete mInterne;
		mInterne = nullptr;
		mChemin = CheminGL::Aucun;
	}

	void Contexte::Echanger() {
		if (mInterne != nullptr)
			glXSwapBuffers(mInterne->affichage, mInterne->fenetre);
	}

	void Contexte::RendreCourant() {
		if (mInterne != nullptr)
			glXMakeCurrent(mInterne->affichage, mInterne->fenetre, mInterne->rc);
	}

	void Contexte::DetacherSurface() {
		// Rien : X11 ne detruit pas la fenetre quand elle passe derriere une
		// autre. La notion d'arriere-plan du bureau n'est pas celle du mobile.
	}

	bool Contexte::RattacherSurface(const nkentseu::NkSurfaceDesc &surface) {
		(void)surface;
		return mInterne != nullptr;
	}

	bool Contexte::PeutDessiner() const {
		return mInterne != nullptr;
	}

	// =========================================================================
	//  ANDROID ET HARMONYOS — EGL, et la même API pour deux systèmes
	// =========================================================================
#elif defined(NKENTSEU_PLATFORM_ANDROID) || defined(NKENTSEU_PLATFORM_HARMONYOS)

	bool Contexte::Creer(const nkentseu::NkSurfaceDesc &surface) {
		if (mInterne != nullptr)
			return false;

		// La SEULE ligne qui diffère entre Android et HarmonyOS de tout ce
		// bloc. Deux systèmes que tout oppose commercialement partagent la
		// même pile graphique, parce que tous deux ont adopté EGL et OpenGL
		// ES. Retenez-le : la portabilité ne suit pas les frontières
		// d'entreprises, elle suit les normes.
#if defined(NKENTSEU_PLATFORM_ANDROID)
		void *fenetreNative = (void *)surface.nativeWindow;
#else
		void *fenetreNative = (void *)surface.ohNativeWindow;
#endif
		if (fenetreNative == nullptr) {
			logger.Error("[contexte] fenetre native absente");
			return false;
		}

		Interne *ctx = new Interne();

		// --- Temps 0 : charger EGL lui-même ----------------------------------
		// Même œuf, troisième poule. eglGetProcAddress est un pointeur de
		// glad, donc nul tant que rien ne l'a chargé. On appelle donc glad
		// SANS display, ce qui le fait passer par dlopen("libEGL.so").
		if (gladLoaderLoadEGL(nullptr) == 0) {
			logger.Error("[contexte] gladLoaderLoadEGL (bootstrap) a echoue");
			delete ctx;
			return false;
		}

		// --- Temps 1 : l'affichage -------------------------------------------
		ctx->affichage = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if (ctx->affichage == EGL_NO_DISPLAY) {
			logger.Error("[contexte] eglGetDisplay a echoue");
			delete ctx;
			return false;
		}
		EGLint majeur = 0, mineur = 0;
		if (eglInitialize(ctx->affichage, &majeur, &mineur) == EGL_FALSE) {
			logger.Error("[contexte] eglInitialize a echoue");
			delete ctx;
			return false;
		}
		// Deuxième passage de glad, AVEC le display cette fois : certaines
		// fonctions EGL n'existent qu'une fois le display connu.
		gladLoaderLoadEGL(ctx->affichage);

		// --- Temps 2 : la configuration --------------------------------------
		const EGLint attributsConfig[] = {EGL_RENDERABLE_TYPE,
										  EGL_OPENGL_ES3_BIT,
										  EGL_SURFACE_TYPE,
										  EGL_WINDOW_BIT,
										  EGL_RED_SIZE,
										  8,
										  EGL_GREEN_SIZE,
										  8,
										  EGL_BLUE_SIZE,
										  8,
										  EGL_ALPHA_SIZE,
										  8,
										  EGL_DEPTH_SIZE,
										  24,
										  EGL_STENCIL_SIZE,
										  8,
										  EGL_NONE};
		EGLint nombre = 0;
		eglChooseConfig(ctx->affichage, attributsConfig, &ctx->config, 1, &nombre);
		if (nombre == 0) {
			// Repli ES2. Des émulateurs entiers ne savent faire que cela, et
			// un livre qui ne prévoit pas le repli laisse la moitié de la
			// classe devant un écran noir.
			const EGLint attributsEs2[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
										   EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
										   EGL_RED_SIZE,        8,
										   EGL_GREEN_SIZE,      8,
										   EGL_BLUE_SIZE,       8,
										   EGL_DEPTH_SIZE,      16,
										   EGL_NONE};
			eglChooseConfig(ctx->affichage, attributsEs2, &ctx->config, 1, &nombre);
		}
		if (nombre == 0) {
			logger.Info("[contexte] eglChooseConfig : aucune configuration ES compatible");
			delete ctx;
			return false;
		}

		// --- Temps 2b : la surface, et les dix essais ------------------------
		// Ceci n'est pas de la superstition, c'est une cicatrice.
		//
		// Juste après le démarrage, l'écran d'accueil du système peut tenir
		// encore quelques dizaines de millisecondes la connexion EGL sur
		// CETTE MÊME fenêtre native. eglCreateWindowSurface échoue alors une
		// fois, avec EGL_BAD_ALLOC, et réussit à l'essai suivant.
		//
		// Un programme qui n'essaie qu'une fois marche sur votre machine et
		// échoue une fois sur trois sur celle de l'utilisateur. C'est la
		// pire catégorie de bogue, et c'est pour cela qu'on écrit la boucle
		// tout de suite.
		for (int essai = 0; essai < 10; ++essai) {
			ctx->surface = eglCreateWindowSurface(ctx->affichage, ctx->config,
												  (EGLNativeWindowType)fenetreNative, nullptr);
			if (ctx->surface != EGL_NO_SURFACE)
				break;
			logger.Infof("[contexte] eglCreateWindowSurface essai %d/10 (0x%x)", essai + 1, (int)eglGetError());
			usleep(50000);
		}
		if (ctx->surface == EGL_NO_SURFACE) {
			logger.Error("[contexte] eglCreateWindowSurface a echoue apres 10 essais");
			delete ctx;
			return false;
		}

		// --- Temps 3 : le contexte -------------------------------------------
		eglBindAPI(EGL_OPENGL_ES_API);
		const EGLint attributsEs3[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
		ctx->rc = eglCreateContext(ctx->affichage, ctx->config, EGL_NO_CONTEXT, attributsEs3);
		if (ctx->rc == EGL_NO_CONTEXT) {
			const EGLint attributsEs2[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
			ctx->rc = eglCreateContext(ctx->affichage, ctx->config, EGL_NO_CONTEXT, attributsEs2);
		}
		if (ctx->rc == EGL_NO_CONTEXT) {
			logger.Error("[contexte] eglCreateContext a echoue");
			eglDestroySurface(ctx->affichage, ctx->surface);
			delete ctx;
			return false;
		}

		// --- Temps 4 : rendre courant ----------------------------------------
		// Notez les DEUX surfaces : celle où l'on dessine et celle où l'on
		// lit. Elles sont identiques ici, et elles ne le seront plus au
		// chapitre 10.
		if (eglMakeCurrent(ctx->affichage, ctx->surface, ctx->surface, ctx->rc) == EGL_FALSE) {
			logger.Error("[contexte] eglMakeCurrent a echoue");
			eglDestroyContext(ctx->affichage, ctx->rc);
			eglDestroySurface(ctx->affichage, ctx->surface);
			delete ctx;
			return false;
		}

		// --- Temps 5 : les fonctions -----------------------------------------
		// gladLoaderLoadGLES2 passe par dlopen("libGLESv2.so"). Le moteur a
		// appris à ses dépens qu'il faut le préférer à
		// gladLoadGLES2(eglGetProcAddress) : sur certains pilotes, le second
		// rend des pointeurs vers une AUTRE implémentation que celle liée à
		// la surface. Le rendu réussit, l'échange réussit, et l'écran reste
		// noir. On garde tout de même le repli.
		int version = gladLoaderLoadGLES2();
		if (version == 0)
			version = gladLoadGLES2((GLADloadfunc)eglGetProcAddress);
		if (version == 0) {
			logger.Error("[contexte] gladLoadGLES2 a echoue");
			eglMakeCurrent(ctx->affichage, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(ctx->affichage, ctx->rc);
			eglDestroySurface(ctx->affichage, ctx->surface);
			delete ctx;
			return false;
		}

		mInterne = ctx;
		mChemin = CheminGL::Egl;
		return true;
	}

	void Contexte::Detruire() {
		if (mInterne == nullptr)
			return;
		eglMakeCurrent(mInterne->affichage, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (mInterne->rc != EGL_NO_CONTEXT)
			eglDestroyContext(mInterne->affichage, mInterne->rc);
		if (mInterne->surface != EGL_NO_SURFACE)
			eglDestroySurface(mInterne->affichage, mInterne->surface);
		delete mInterne;
		mInterne = nullptr;
		mChemin = CheminGL::Aucun;
	}

	void Contexte::Echanger() {
		if (mInterne == nullptr || mInterne->surface == EGL_NO_SURFACE)
			return;
		// ON REGARDE CE QUE L'ECHANGE REND. Sans ce test, une surface morte
		// produit des centaines d'erreurs par seconde dans le journal du
		// systeme, l'ecran reste noir, et rien dans le programme ne le
		// remarque. C'est exactement ce qui a ete observe sur MEmu le
		// 10 septembre 2026, en basculant vers une autre application.
		if (eglSwapBuffers(mInterne->affichage, mInterne->surface) == EGL_FALSE) {
			const EGLint faute = eglGetError();
			if (faute == EGL_BAD_SURFACE || faute == EGL_BAD_NATIVE_WINDOW) {
				logger.Warnf("[contexte] surface perdue (0x%x) : on attend le rattachement",
							(int)faute);
				DetacherSurface();
			}
		}
	}

	void Contexte::RendreCourant() {
		if (mInterne != nullptr)
			eglMakeCurrent(mInterne->affichage, mInterne->surface, mInterne->surface, mInterne->rc);
	}

	// ── LE SEUL CHEMIN OU CES TROIS METHODES FONT QUELQUE CHOSE ────────────
	void Contexte::DetacherSurface() {
		if (mInterne == nullptr || mInterne->surface == EGL_NO_SURFACE)
			return;
		// Detacher AVANT de detruire. Detruire une surface courante est un
		// comportement indefini, et c'est la meme regle qu'au temps 4 de la
		// creation : on ne supprime jamais une ressource liee.
		eglMakeCurrent(mInterne->affichage, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroySurface(mInterne->affichage, mInterne->surface);
		mInterne->surface = EGL_NO_SURFACE;
		logger.Warn("[contexte] surface detachee (application en arriere-plan)");
	}

	bool Contexte::RattacherSurface(const nkentseu::NkSurfaceDesc &surface) {
		if (mInterne == nullptr)
			return false;

#if defined(NKENTSEU_PLATFORM_ANDROID)
		void *fenetreNative = (void *)surface.nativeWindow;
#else
		void *fenetreNative = (void *)surface.ohNativeWindow;
#endif
		if (fenetreNative == nullptr) {
			logger.Info("[contexte] rattachement : pas encore de fenetre native");
			return false;
		}

		// Si une surface traine encore, on s'en debarrasse : la fenetre native
		// a change, la surface qui la decrivait ne vaut plus rien.
		if (mInterne->surface != EGL_NO_SURFACE)
			DetacherSurface();

		// Meme boucle de reessais qu'a la creation, et pour une raison voisine :
		// au retour d'arriere-plan, le systeme peut ne pas avoir fini de rendre
		// la fenetre.
		for (int essai = 0; essai < 10; ++essai) {
			mInterne->surface = eglCreateWindowSurface(mInterne->affichage, mInterne->config,
													  (EGLNativeWindowType)fenetreNative, nullptr);
			if (mInterne->surface != EGL_NO_SURFACE)
				break;
			usleep(50000);
		}
		if (mInterne->surface == EGL_NO_SURFACE) {
			logger.Errorf("[contexte] rattachement impossible (0x%x)", (int)eglGetError());
			return false;
		}

		if (eglMakeCurrent(mInterne->affichage, mInterne->surface, mInterne->surface, mInterne->rc) == EGL_FALSE) {
			logger.Error("[contexte] eglMakeCurrent apres rattachement a echoue");
			return false;
		}
		logger.Info("[contexte] surface rattachee");
		return true;
	}

	bool Contexte::PeutDessiner() const {
		return mInterne != nullptr && mInterne->surface != EGL_NO_SURFACE;
	}

	// =========================================================================
	//  LE WEB — WebGL, où quatre des cinq temps disparaissent
	// =========================================================================
#elif defined(NKENTSEU_PLATFORM_EMSCRIPTEN)

	bool Contexte::Creer(const nkentseu::NkSurfaceDesc &surface) {
		if (mInterne != nullptr)
			return false;

		Interne *ctx = new Interne();

		// --- Temps 1, 2 et 3 en un seul appel --------------------------------
		// Le navigateur ne vous laisse pas choisir un format de pixel, ni
		// créer un contexte provisoire, ni négocier une version. Il vous
		// donne un contexte WebGL 2 sur un élément <canvas> ou il refuse.
		//
		// C'est la plateforme la plus SIMPLE des cinq, et c'est parce que
		// c'est la plus CONTRAINTE. Souvenez-vous-en au chapitre 12 : une
		// interface qui offre moins de choix est plus facile à implémenter.
		EmscriptenWebGLContextAttributes attributs;
		emscripten_webgl_init_context_attributes(&attributs);
		attributs.alpha = false;
		attributs.depth = true;
		attributs.stencil = true;
		attributs.antialias = false;
		attributs.premultipliedAlpha = false;
		attributs.preserveDrawingBuffer = false;
		attributs.enableExtensionsByDefault = true;
		// WebGL 2, c'est-à-dire OpenGL ES 3.0. WebGL 1 ne suffirait pas au
		// chapitre 8, et un repli qui échoue plus tard vaut moins qu'un
		// refus franc maintenant.
		attributs.majorVersion = 2;
		attributs.minorVersion = 0;

		// Le descripteur de surface, sur le Web, contient UN champ : le
		// sélecteur CSS du canevas. Un dièse et un nom. Comparez avec les
		// six champs de Wayland.
		const char *canevas = surface.canvasId != nullptr ? surface.canvasId : "#canvas";

		ctx->rc = emscripten_webgl_create_context(canevas, &attributs);
		if (ctx->rc <= 0) {
			logger.Errorf("[contexte] emscripten_webgl_create_context a echoue (canevas '%s')", canevas);
			delete ctx;
			return false;
		}

		// --- Temps 4 -----------------------------------------------------------
		if (emscripten_webgl_make_context_current(ctx->rc) != EMSCRIPTEN_RESULT_SUCCESS) {
			logger.Error("[contexte] emscripten_webgl_make_context_current a echoue");
			emscripten_webgl_destroy_context(ctx->rc);
			delete ctx;
			return false;
		}

		// --- Temps 5 -----------------------------------------------------------
		if (gladLoadGLES2((GLADloadfunc)emscripten_webgl_get_proc_address) == 0) {
			logger.Error("[contexte] gladLoadGLES2 a echoue");
			emscripten_webgl_make_context_current(0);
			emscripten_webgl_destroy_context(ctx->rc);
			delete ctx;
			return false;
		}

		mInterne = ctx;
		mChemin = CheminGL::WebGL;
		return true;
	}

	void Contexte::Detruire() {
		if (mInterne == nullptr)
			return;
		emscripten_webgl_make_context_current(0);
		if (mInterne->rc > 0)
			emscripten_webgl_destroy_context(mInterne->rc);
		delete mInterne;
		mInterne = nullptr;
		mChemin = CheminGL::Aucun;
	}

	// L'ÉCHANGE N'EXISTE PAS SUR LE WEB, et ce n'est pas un oubli.
	//
	// Le navigateur présente le canevas quand votre fonction rend la main,
	// pas quand vous le demandez. Il n'y a donc rien à appeler ici. La
	// conséquence est plus grave qu'elle en a l'air : votre boucle de jeu ne
	// peut pas être un while, elle doit être une fonction rappelée par le
	// navigateur, et c'est l'exercice 11.
	void Contexte::Echanger() {}

	void Contexte::RendreCourant() {
		if (mInterne != nullptr)
			emscripten_webgl_make_context_current(mInterne->rc);
	}

	void Contexte::DetacherSurface() {
		// Rien. Un onglet en arriere-plan garde son contexte ; le navigateur
		// se contente de ne plus appeler la boucle de rendu.
	}

	bool Contexte::RattacherSurface(const nkentseu::NkSurfaceDesc &surface) {
		(void)surface;
		return mInterne != nullptr;
	}

	bool Contexte::PeutDessiner() const {
		return mInterne != nullptr;
	}

	// =========================================================================
	//  TOUT LE RESTE — Wayland, XCB, macOS, iOS
	// =========================================================================
#else

	// Ce bloc existe pour que le fichier COMPILE partout, et il refuse
	// honnêtement de fonctionner. C'est la leçon du backend Noop du
	// chapitre 3 : un portage commence par un échec propre, jamais par une
	// erreur de compilation.
	//
	// Wayland est l'exercice 12. Prévenez-vous : le descripteur y a six
	// champs, il faut une wl_egl_window intermédiaire, et rien ne s'affiche
	// tant que le compositeur n'a pas envoyé son événement de
	// configuration.
	bool Contexte::Creer(const nkentseu::NkSurfaceDesc &surface) {
		(void)surface;
		logger.Info("[contexte] aucun chemin OpenGL pour ce systeme (voir l'exercice 12)");
		return false;
	}

	void Contexte::Detruire() {
		delete mInterne;
		mInterne = nullptr;
		mChemin = CheminGL::Aucun;
	}

	void Contexte::Echanger() {}
	void Contexte::RendreCourant() {}

	void Contexte::DetacherSurface() {}

	bool Contexte::RattacherSurface(const nkentseu::NkSurfaceDesc &surface) {
		(void)surface;
		return false;
	}

	bool Contexte::PeutDessiner() const {
		return false;
	}

#endif

	// =========================================================================
	//  Ce qui est commun aux cinq : interroger le pilote.
	// =========================================================================

	nkentseu::NkString Contexte::VersionGL() const {
		if (mInterne == nullptr)
			return nkentseu::NkString();
#if defined(NKENTSEU_PLATFORM_WINDOWS) || defined(NKENTSEU_WINDOWING_XLIB) ||                                          \
	defined(NKENTSEU_PLATFORM_ANDROID) || defined(NKENTSEU_PLATFORM_HARMONYOS) ||                                      \
	defined(NKENTSEU_PLATFORM_EMSCRIPTEN)
		const char *s = (const char *)glGetString(GL_VERSION);
		return s != nullptr ? nkentseu::NkString(s) : nkentseu::NkString();
#else
		return nkentseu::NkString();
#endif
	}

	nkentseu::NkString Contexte::Carte() const {
		if (mInterne == nullptr)
			return nkentseu::NkString();
#if defined(NKENTSEU_PLATFORM_WINDOWS) || defined(NKENTSEU_WINDOWING_XLIB) ||                                          \
	defined(NKENTSEU_PLATFORM_ANDROID) || defined(NKENTSEU_PLATFORM_HARMONYOS) ||                                      \
	defined(NKENTSEU_PLATFORM_EMSCRIPTEN)
		const char *s = (const char *)glGetString(GL_RENDERER);
		return s != nullptr ? nkentseu::NkString(s) : nkentseu::NkString();
#else
		return nkentseu::NkString();
#endif
	}

	nkentseu::NkString Contexte::Decrire() const {
		if (mInterne == nullptr)
			return nkentseu::NkString("contexte absent");
		return nkentseu::NkString(NomDuChemin(mChemin)) + " | " + VersionGL() + " | " + Carte();
	}

} // namespace monrhi
