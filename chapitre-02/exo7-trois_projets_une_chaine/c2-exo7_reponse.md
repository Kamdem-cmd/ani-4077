# ANI-4067 — Chapitre 02 : Exercice — Dépendances Transitives et Ordre de Liaison

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem
* **Cadre :** Compte-rendu d'exercice sur l'édition de liens (*linking*)

---

## 1. Configuration du Projet (`.jenga`)

Mise en place de l'espace de travail contenant la bibliothèque utilitaire `MonUtil`, la bibliothèque RHI `MonRhi` (qui dépend de `MonUtil`), et l'application `Essai`.

```python
from Jenga import *

with workspace("MonRhiWks", location="."):
    configurations(["Debug", "Release"])

    # 1. Bibliothèque utilitaire de bas niveau
    with project("MonUtil"):
        staticlib()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/MonUtil/**.cpp"])
        includedirs(["src"])
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")

    # 2. Bibliothèque RHI (dépend de MonUtil)
    with project("MonRhi"):
        staticlib()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/MonRhi/**.cpp"])
        includedirs(["src"])
        links(["MonUtil"])
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")

    # 3. Application cliente
    with project("Essai"):
        consoleapp()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/Essai/**.cpp"])
        includedirs(["src"])
        libdirs(["%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}"])
        links(["MonRhi", "MonUtil"])  # Déclarations des bibliothèques à lier
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```

## Reponses

* **Non. J'ai pas eu besoin de faire appel à Util directement dans Essai parce qu'il y a deja un intermediaire qui le fait. MonRhi.**

## Dans l'autre sans


**Réponse : Oui.Explication Technique :Absence d'imbrication des bibliothèques statiques : Une bibliothèque statique (.lib sous Windows ou .a sous Linux) n'est qu'une simple archive de fichiers objets (.obj). Lorsque MonRhi.lib est construite, elle ne fusionne pas le code binaire de MonUtil.lib à l'intérieur d'elle-même.Résolution globale des symboles à l'édition de liens : C'est seulement lors de la création de l'exécutable final (Essai.exe) que le lieur rassemble le code binaire. Lorsque le lieur analyse MonRhi.obj, il rencontre des appels vers des fonctions situées dans MonUtil. Si MonUtil.lib n'est pas transmise explicitement au lieur via la directive links, ces symboles restent indéfinis.Gestion par le système de build : À moins que le système de build ne gère nativement la propagation transitive automatique des dépendances graphiques (comme le fait CMake avec PUBLIC/PRIVATE), l'exécutable doit déclarer toutes les archives de bibliothèques nécessaires à la résolution complète du graphe de symboles.3. Analyse 2 : Inversion de l'Ordre dans linksQuestion : Testez ensuite en inversant l'ordre dans links.** 

### Que se passe-t-il, et sur quel système ?Configuration de test :Modifications apportées au projet Essai dans le fichier .jenga :
```Python
links(["MonUtil", "MonRhi"])  # Ordre inversé
```

**Résultats observés selon la chaîne de compilation :Système / Chaîne de CompilationEditeur de Liens (Linker)Résultat obtenu avec ["MonUtil", "MonRhi"]Explication du comportementGCC / Clang (Linux, macOS, MSYS2)GNU ld / lld ÉCHEC DE BUILD (undefined reference)Passe unique (Single-Pass) : ld parcourt les bibliothèques strictement de gauche à droite. Lorsqu'il analyse MonUtil, aucun symbole n'a encore été réclamé par Essai (car MonRhi n'a pas encore été lue). MonUtil est ignorée. Quand MonRhi est analysée ensuite, le besoin de symboles de MonUtil apparaît, mais il est trop tard.Windows (MSVC)Microsoft link.exe  SUCCÈSPasses multiples (Multi-Pass) : Le lieur de Visual Studio effectue des passages répétés sur la liste des bibliothèques transmises jusqu'à ce que tous les symboles dépendants soient résolus, indépendamment de leur ordre de déclaration.4. Synthèse PédagogiqueRègle d'or pour la compatibilité multiplateforme (GCC/Clang/MSVC) : Les bibliothèques statiques doivent toujours être listées de la plus haute abstraction vers la plus basse abstraction (du composant utilisateur vers la dépendance fondamentale) :$$\text{Ordre obligatoire : } \text{Application} \longrightarrow \text{MonRhi} \longrightarrow \text{MonUtil}$$L'inversion de l'ordre est une source classique d'erreurs de build qui n'apparaissent que sur certains systèmes d'exploitation (typiquement sous Linux ou MSYS2/MinGW) alors que le code compile sans problème sous Visual Studio.**