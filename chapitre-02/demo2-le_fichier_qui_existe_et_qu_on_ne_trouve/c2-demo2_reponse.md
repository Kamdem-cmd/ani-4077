# ANI-4067 — Chapitre 02 : demo 2

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
# cannot find -lMonRhi

* **fichier `.jenga`:**
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
✓   [1/1] Compiled: MonRhi.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\MonRhi.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.78s  │
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
│  ✗ Build Failed                                                                 Time: 0.94s  │
│ Errors: 1  | Failed files: 1                                                                 │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                  BUILD FAILED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/2
Failed:         1
Errors:         1
Time:           1.72s
Status:         ✗ FAILURE
════════════════════════════════════════════════════════════════════════════════

Echecs (1) — a corriger :
  ✗ Essai
```