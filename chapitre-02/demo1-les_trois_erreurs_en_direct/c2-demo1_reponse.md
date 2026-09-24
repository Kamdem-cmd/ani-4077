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

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\MonRhi.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 6.77s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\Essai\Essai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 2.06s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           8.84s
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

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\MonRhi.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.75s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\Essai\Essai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.82s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           1.57s
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

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\MonRhi.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.76s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║                                Compilation Error: Link Failed                                ║
╠══════════════════════════════════════════════════════════════════════════════════════════════╣
║ C:/msys64/ucrt64/bin/ld: cannot find -lMonRhi: No such file or directory                     ║
║ clang++: error: linker command failed with exit code 1 (use -v to see invocation)            ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝
✗ Link failed: Build\Bin\Debug-Windows\Essai\Essai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✗ Build Failed                                                                 Time: 0.85s  │
│ Errors: 1  | Failed files: 1                                                                 │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                  BUILD FAILED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/2
Failed:         1
Errors:         1
Time:           1.62s
Status:         ✗ FAILURE
════════════════════════════════════════════════════════════════════════════════

Echecs (1) — a corriger :
  ✗ Essai
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

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\MonRhi.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.75s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\Essai\Essai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.98s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           1.74s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

## QUI PARLE NORMALEMENT ?

* **includedirs manquant :** c'est le `compilateur` qui parle.
* **libdirs manquant :** c'est le `lieur` qui parle.
* **links manquant :** c'est le `lieur` qui parle.