# ANI-4067 — Chapitre 02 : Exercice 2

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
# Espace de la commande
```powershell
from Jenga import *

# with workspace("MonRhiWks", location="."):
configurations(["Debug", "Release"])

# La bibliotheque : du code compile, sans main.
with project("MonRhi"):
    staticlib()
    language("C++")
    cppdialect("C++17")
    location(".")
    files(["src/MonRhi/**.cpp"])
    includedirs(["src"])
    objdir("%{wks.location}/Build/Obj/"
            "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    targetdir("%{wks.location}/Build/Lib/"
                "%{cfg.buildcfg}-%{cfg.system}")

# Le programme qui s'en sert.
with project("Essai"):
    consoleapp()
    language("C++")
    cppdialect("C++17")
    location(".")
    files(["src/Essai/**.cpp"])
    includedirs(["src"])
    libdirs(["%{wks.location}/Build/Lib/"
                "%{cfg.buildcfg}-%{cfg.system}"])
    links(["MonRhi"])
    objdir("%{wks.location}/Build/Obj/"
            "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    targetdir("%{wks.location}/Build/Bin/"
                "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```

# Resultat de la commande `jenga info`


```powershell
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

No .jenga workspace file found.
```
___
# Resultat de la commande `jenga build`


```powershell
D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga build

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

No .jenga workspace file found.
```