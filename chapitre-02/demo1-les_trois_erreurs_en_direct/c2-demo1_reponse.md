# ANI-4067 — Chapitre 02 : Exercice 1

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
# Les Trois erreurs en Direct

* **L'Espace de travail initial:**
```python
from Jenga import *

with workspace("MonRhiWks", location="."):
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

### Resultat
```Powershell
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

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (2 projects):
  1. MonRhi [STATIC_LIB] →
  2. Essai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonRhi                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project MonRhi

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project Essai

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.03s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```
* **Premiere modification:**
```python
from Jenga import *

with workspace("MonRhiWks", location="."):
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
        #includedirs(["src"])
        libdirs(["%{wks.location}/Build/Lib/"
                 "%{cfg.buildcfg}-%{cfg.system}"])
        links(["MonRhi"])
        objdir("%{wks.location}/Build/Obj/"
               "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Bin/"
                  "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```

### Resultat
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

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (2 projects):
  1. MonRhi [STATIC_LIB] →
  2. Essai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonRhi                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project MonRhi

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project Essai

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.01s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

* **Seconde Modification:**
```python
from Jenga import *

with workspace("MonRhiWks", location="."):
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
        #libdirs(["%{wks.location}/Build/Lib/"
                #  "%{cfg.buildcfg}-%{cfg.system}"])
        links(["MonRhi"])
        objdir("%{wks.location}/Build/Obj/"
               "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Bin/"
                  "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```
### Resultat :

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

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (2 projects):
  1. MonRhi [STATIC_LIB] →
  2. Essai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonRhi                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project MonRhi

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project Essai

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.01s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

* **Troisieme modification:**
```python
from Jenga import *

with workspace("MonRhiWks", location="."):
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
        #links(["MonRhi"])
        objdir("%{wks.location}/Build/Obj/"
               "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Bin/"
                  "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```
### Resultat :

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

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (2 projects):
  1. MonRhi [STATIC_LIB] →
  2. Essai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonRhi                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project MonRhi

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

No source files found for project Essai

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.02s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```