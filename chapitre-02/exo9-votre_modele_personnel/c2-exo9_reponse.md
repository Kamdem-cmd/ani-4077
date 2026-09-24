# Fiche de Survie `.jenga` — Modèle Générique Commenté

---

### Informations
* **Fichier :** `<NOM_PROJET>.jenga`
* **Usage :** Trame réutilisable pour la création de modules, bibliothèques et applications C++ avec Jenga.

---

## Modèle de Script `.jenga`

```python
from Jenga import *

# ==============================================================================
# 1. ESPACE DE TRAVAIL (WORKSPACE)
# ==============================================================================
# RÔLE : Déclare le conteneur racine qui regroupe les projets à compiler.
# SI OUBLIÉ : "Projects Built: 0/0" -> Échec silencieux. Le script est lu sans erreur,
#              mais aucun binaire (.exe/.lib) n'est généré.
with workspace("<NOM_DU_WORKSPACE>", location="."):

    # RÔLE : Définit les variantes de compilation accessibles dans le projet.
    # SI OUBLIÉ : Erreur lors de la résolution des variables de chemin (%{cfg.buildcfg})
    #              ou compilation forcée sur une configuration indéterminée.
    configurations(["Debug", "Release"])

    # ==========================================================================
    # 2. DÉCLARATION D'UN MODULE / PROJET
    # ==========================================================================
    # RÔLE : Crée une cible de compilation autonome (Application ou Bibliothèque).
    # SI OUBLIÉ : Le code source situé dans le dossier ne sera ni analysé ni compilé.
    with project("<NOM_DU_PROJET>"):

        # --- TYPE DE CIBLE (Activer une seule option selon le besoin) ---
        staticlib()       # Génère une bibliothèque statique (.lib / .a)
        # consoleapp()    # Génère un exécutable console (.exe)
        # sharedlib()     # Génère une bibliothèque dynamique (.dll / .so / .dylib)
        # RÔLE : Indique au système de build et au lieur quel type de binaire produire.
        # SI OUBLIÉ : Erreur de génération des règles de liaison (type de cible indéfini).

        # RÔLE : Définit le langage source principal du projet.
        # SI OUBLIÉ : Mauvaise sélection des compilateurs (ex: appel de gcc au lieu de g++).
        language("C++")

        # RÔLE : Fixe la norme ISO du langage C++.
        # SI OUBLIÉ : Compilation avec la norme C++ par défaut du système (ex: C++11/14).
        #              Provoque des erreurs de syntaxe sur le code moderne :
        #              "error: 'std::filesystem' has not been declared"
        cppdialect("C++17")

        # RÔLE : Définit le dossier racine de référence du projet.
        # SI OUBLIÉ : Résolution incorrecte des chemins relatifs pour les sources et includes.
        location(".")

        # --- EN-TÊTES PRÉCOMPILÉS (PCH) [OPTIONNEL] ---
        # RÔLE : Accélère la compilation en précompilant les en-têtes lourds.
        # SI OUBLIÉ : Pas d'erreur directe, mais temps de compilation plus longs.
        #              (Si le code C++ contient `#include "pch.h"` sans PCH configuré :
        #              "fatal error: pch.h: No such file or directory").
        pchheader("src/pch.h")
        pchsource("src/pch.cpp")

        # --- FICHIERS SOURCES ---
        # RÔLE : Liste les fichiers source (.cpp) et d'en-tête (.h) à inclure au build.
        # SI OUBLIÉ : Le compilateur n'a rien à traiter :
        #              "No input files" ou "undefined reference to 'main'".
        files([
            "src/**.cpp",
            "src/**.h"
        ])

        # --- CHEMINS D'INCLUSION (INCLUDES) ---
        # RÔLE : Ajoute les dossiers aux chemins de recherche pour la directive `#include <...>`.
        # SI OUBLIÉ : "fatal error: MonEnTete.h: No such file or directory" à la compilation.
        includedirs([
            "src",
            "../<AUTRE_MODULE>/src"
        ])

        # --- CHEMINS DES BIBLIOTHÈQUES (LIBDIRS) ---
        # RÔLE : Indique au lieur (linker) où trouver les fichiers archives (.lib / .a).
        # SI OUBLIÉ : "cannot find -l<NOM_MODULE>" ou "LNK1104: cannot open file '.lib'".
        libdirs([
            "%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}"
        ])

        # --- DÉPENDANCES ET LIAISON (LINKS) ---
        # RÔLE : Transmet les bibliothèques nécessaires à la résolution des symboles.
        # SI OUBLIÉ : "undefined reference to 'MaFonction()'" ou
        #              "LNK2019: unresolved external symbol".
        links([
            "<NOM_MODULE_DEPENDANCE>"
        ])

        # --- DEFINES PRÉPROCESSEUR ---
        # RÔLE : Injecte des macros globales au niveau du préprocesseur C++.
        # SI OUBLIÉ : Invalidation des conditions `#ifdef`, entraînant du code mort ou
        #              des erreurs de compilation si des types dépendent de ces macros.
        defines([
            "<NOM_MACRO_DEFINITIONS>"
        ])

        # --- DOSSIERS DE SORTIE ---
        # RÔLE : Spécifie le répertoire pour les fichiers objets temporaires (.obj / .o).
        # SI OUBLIÉ : Pollution du dossier source par les fichiers objets temporaires et
        #              risques de conflits entre les configurations Debug et Release.
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")

        # RÔLE : Spécifie le répertoire final de génération des binaires (.exe / .lib / .dll).
        # SI OUBLIÉ : Génération du binaire à la racine du projet, rendant le nettoyage
        #              et l'organisation du projet difficiles.
        targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")

        # ======================================================================
        # 3. FILTRES DE PLATEFORME ET DE CONFIGURATION
        # ======================================================================
        # RÔLE : Applique des options strictement réservées à un OS spécifique.
        # SI OUBLIÉ : Inclusion de code spécifique (Win32, POSIX, Cocoa) sur le mauvais OS :
        #              "error: unknown type name 'HWND'" sous Linux/macOS.
        with filter("system:Windows"):
            defines(["WIN32_LEAN_AND_MEAN"])

        # RÔLE : Active le mode débogage (non optimisé, symboles complets).
        # SI OUBLIÉ : Impossibilité d'attacher un débogueur ou de placer des points d'arrêt.
        with filter("config:Debug"):
            optimize("Off")
            symbols(True)
            defines(["_DEBUG", "DEBUG"])

        # RÔLE : Active les optimisations de performance pour la livraison/production.
        # SI OUBLIÉ : Binaire final inutilement lourd et lent en exécution.
        with filter("config:Release"):
            optimize("Speed")
            symbols(False)
            defines(["NDEBUG"])