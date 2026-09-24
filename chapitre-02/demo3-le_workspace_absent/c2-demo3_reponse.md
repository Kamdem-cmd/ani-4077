# ANI-4067 — Chapitre 02 : Démonstration — Les Échecs Silencieux

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem
* **Cadre :** Démonstration pédagogique en classe

---

## 1. Fichier de Configuration de la Démonstration (`.jenga`)

Le fichier de configuration `.jenga` ci-dessous est présenté et projeté devant la classe. La directive d'espace de travail (`workspace`) y a été intentionnellement commentée afin de séparer la déclaration des projets de leur enregistrement dans le graphe de construction global.

```python
from Jenga import *

# La ligne de workspace est volontairement commentée pour la démonstration :
# with workspace("MonRhiWks", location="."):

configurations(["Debug", "Release"])

# Déclaration de la bibliothèque
with project("MonRhi"):
    staticlib()
    language("C++")
    cppdialect("C++17")
    location(".")
    files(["src/MonRhi/**.cpp"])
    includedirs(["src"])
    objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")

# Déclaration du programme exécutable
with project("Essai"):
    consoleapp()
    language("C++")
    cppdialect("C++17")
    location(".")
    files(["src/Essai/**.cpp"])
    includedirs(["src"])
    libdirs(["%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}"])
    links(["MonRhi"])
    objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    targetdir("%{wks.location}/Build/Bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```
## 2. Relevé d'Exécution de la Commande jenga build
La commande de construction est lancée dans le terminal sous les yeux des étudiants :

```PowerShell
D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI> jenga build

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Parsing .jenga configuration... OK
Building targets...

Projects Built: 0/0
```
## 3. Mise en Scène et Déroulement Pédagogique
Projection du résultat : L'écran affiche le bilan final ci-dessus. Les lignes Parsing .jenga configuration... OK et Projects Built: 0/0 sont clairement lisibles par l'ensemble de la classe.

Question posée à l'assemblée :

« Regardez la sortie du terminal. Où est le message d'erreur ? »

Constat et réaction de la classe :
Les étudiants cherchent un message d'erreur en rouge, une exception Python ou une alerte de compilation. Il n'y en a aucun. Le processus s'interrompt proprement avec un code de succès (exit status 0). L'outil annonce sereinement avoir construit zéro projet sur zéro sans émettre la moindre protestation.

## 4. Analyse Technique et Portée Pédagogique
Pourquoi Projects Built: 0/0 et non pas une erreur ?
Validité du script : Le fichier .jenga est un script Python parfaitement valide. L'interpréteur l'exécute sans erreur de syntaxe.

Projets orphelins : Les blocs project("MonRhi") et project("Essai") créent des objets en mémoire, mais en l'absence du bloc workspace(...), aucun projet n'est enregistré dans la liste des cibles à générer.

Absence de cible : L'outil jenga parcourt l'espace de travail principal. N'en trouvant aucun, il conclut qu'il y a 0 projet à construire et termine sa tâche avec succès.

La leçon des échecs silencieux (Silent Failures)
Ce test illustre la première leçon fondamentale sur la fiabilité des chaînes de construction :

La différence entre un avertissement et un échec silencieux : Un message indiquant "No .jenga file found" est une erreur explicite signalant un fichier manquant. L'affichage Projects Built: 0/0, en revanche, est un échec silencieux : le système confirme que tout s'est "bien passé" alors que rien n'a été produit.

Le risque en ingénierie : Un développeur qui ne vérifie pas le nombre de projets construits peut croire à tort que son code C++ a été compilé et validé, alors qu'aucun binaire (.exe ou .lib) n'a été généré dans le dossier de sortie.