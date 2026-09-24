# ANI-4067 — Chapitre 02 : Exercice — Portabilité et Compilation Cross-Platform (Windows vs Linux)

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem
* **Cadre :** Comparaison des comportements de build entre Windows (MSVC) et Linux/WSL (GCC/Clang)

---

## 1. Relevé des Différences Observées (Windows vs Linux)

Sans modifier le fichier `.jenga` initial, la compilation de l'espace de travail sous un environnement Linux / WSL révèle les divergences système suivantes :

| Élément de Comparaison | Environnement Windows | Environnement Linux / WSL | Impact & Analyse |
| :--- | :--- | :--- | :--- |
| **Binaire Bibliothèque** | `MonRhi.lib` | `libMonRhi.a` | Jenga/GCC préfixe automatiquement les bibliothèques statiques par `lib` et utilise l'extension `.a`. |
| **Binaire Exécutable** | `Essai.exe` | `Essai` *(sans extension)* | Sous Linux, le binaire exécutable n'a pas d'extension de fichier mais possède le bit d'exécution (`+x`). |
| **Arborescence de Sortie** | `.../Debug-Windows/...` | `.../Debug-Linux/...` | La macro `%{cfg.system}` résout dynamiquement le système hôte, évitant tout chevauchement de binaires. |
| **Comportement du Lieur** | Tolère l'ordre inversé `links(["MonUtil", "MonRhi"])` | Échoue avec `undefined reference` si l'ordre est inversé | Le lieur MSVC (`link.exe`) réalise plusieurs passes, alors que GNU `ld` réalise une passe unique de gauche à droite. |
| **Dépendances Système** | Définitions `WIN32_LEAN_AND_MEAN` requises | Bibliothèque POSIX `pthread` parfois nécessaire | Les API système bas niveau diffèrent et nécessitent des drapeaux préprocesseur spécifiques. |

---

## 2. Liste des Ajustements Effectués pour la Portabilité

Afin d'obtenir un script unique et fonctionnel sur les deux plateformes sans multiplier les conditions (`filter`), les règles suivantes ont été appliquées :

1. **Ordre strict des dépendances (Agnostique) :**
   Alignement des `links` selon la règle du lieur GNU : **de la plus haute abstraction vers la plus basse** (`links(["MonRhi", "MonUtil"])`). Cet ordre est universel : il satisfait GNU `ld` sous Linux tout en restant 100 % valide sous MSVC.
2. **Utilisation des variables de motifs Jenga :**
   Conservation des macros `%{cfg.system}` et `%{cfg.buildcfg}` dans `objdir` et `targetdir` pour garantir une séparation propre des artefacts de compilation selon l'OS sans écrire de règles manuelles.
3. **Isolation conditionnelle minimale (2 filtres uniquement) :**
   * Un filtre `system:Windows` réservé aux macros spécifiques à l'API Win32 (`WIN32_LEAN_AND_MEAN`).
   * Un filtre `system:Linux` pour l'ajout du thread POSIX (`pthread`) si nécessaire.

---

## 3. Fichier `.jenga` Final Multiplateforme

```python
from Jenga import *

with workspace("MonRhiWks", location="."):
    configurations(["Debug", "Release"])

    # --------------------------------------------------------------------------
    # 1. Module MonUtil (Bibliothèque de bas niveau)
    # --------------------------------------------------------------------------
    with project("MonUtil"):
        staticlib()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/MonUtil/**.cpp"])
        includedirs(["src"])
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")

    # --------------------------------------------------------------------------
    # 2. Module MonRhi (Bibliothèque dépendante de MonUtil)
    # --------------------------------------------------------------------------
    with project("MonRhi"):
        staticlib()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/MonRhi/**.cpp"])
        includedirs(["src"])
        links(["MonUtil"])  # Dépendance explicite vers MonUtil
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")

    # --------------------------------------------------------------------------
    # 3. Application Essai (Exécutable client)
    # --------------------------------------------------------------------------
    with project("Essai"):
        consoleapp()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/Essai/**.cpp"])
        includedirs(["src"])
        libdirs(["%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}"])
        
        # Ordre de liaison strict (du plus haut niveau au plus bas niveau)
        # Indispensable pour la passe unique de GNU ld sous Linux
        links(["MonRhi", "MonUtil"])
        
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")

        # --- Filtres spécifiques aux plateformes (Ajustements minimaux) ---
        with filter("system:Windows"):
            defines(["WIN32_LEAN_AND_MEAN"])

        with filter("system:Linux"):
            links(["pthread"])

        # --- Configurations d'optimisation ---
        with filter("config:Debug"):
            optimize("Off")
            symbols(True)
            defines(["_DEBUG", "DEBUG"])

        with filter("config:Release"):
            optimize("Speed")
            symbols(False)
            defines(["NDEBUG"])

```

## 4. Bilan de Portabilité
**Grâce à ce fichier unifié :**

* Sous Windows, l'exécution produit Build/Bin/Debug-Windows/Essai/Essai.exe.

* Sous Linux / WSL, l'exécution produit Build/Bin/Debug-Linux/Essai/Essai.

* Aucune modification de code ou de script n'est requise lors du passage d'un système à l'autre (git pull puis jenga build direct).