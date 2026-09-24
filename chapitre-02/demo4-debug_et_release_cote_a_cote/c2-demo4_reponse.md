# ANI-4067 — Chapitre 02 : Demo 4

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
# Debug VS Realese
* **Fichiers**
`Essai`
```cpp
#include "MonRhi/MonRhi.h"
#include <iostream>

int main() {
    long long result = MonRhi::ComputeLongSum();

    std::cout << "Somme calculee : " << result << std::endl;

    return 0;
}
```

* **Fichiers**
`MonRhi`
```cpp
#include "MonRhi.h"

namespace MonRhi {
    long long ComputeLongSum() {
        long long sum = 0;
        for (long long i = 1; i <= 100000000; ++i) {
            sum += i;
        }
        return sum;
    }
}
```

* **1. En Realese :**

```powershell
D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga build --config release

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

Configuration: release
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (2 projects):
  1. MonRhi [STATIC_LIB] →
  2. Essai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonRhi                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 2 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.06s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓ All files up to date
ℹ Linking...
✓ Built: Build\Bin\release-Windows\Essai\Essai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.19s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.25s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════


D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga run --config release

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


━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  Essai.exe
     D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Bin\release-Windows\Essai\Essai.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Somme calculee : 5000000050000000

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (0.39s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```
* **2. En Debug :**

```powershell
D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga clean

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

Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Obj\Debug-Windows\MonRhi\src_MonRhi_Inutile.obj
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Obj\Debug-Windows\MonRhi\src_MonRhi_MonRhi.obj
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Lib\Debug-Windows\MonRhi.lib
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Obj\Debug-Windows\Essai\src_Essai_main.obj
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Bin\Debug-Windows\Essai\Essai.exe
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Obj\Release-Windows\MonRhi\src_MonRhi_Inutile.obj
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Obj\Release-Windows\MonRhi\src_MonRhi_MonRhi.obj
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Lib\Release-Windows\MonRhi.lib
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Obj\Release-Windows\Essai\src_Essai_main.obj
Removed D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Bin\Release-Windows\Essai\Essai.exe

D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga build --config debug

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

Configuration: debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (2 projects):
  1. MonRhi [STATIC_LIB] →
  2. Essai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonRhi                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 2 source file(s)
✓   [1/2] Compiled: MonRhi.cpp
✓   [2/2] Compiled: Inutile.cpp
ℹ Linking...
✓ Built: Build\Lib\debug-Windows\MonRhi.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.83s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: Essai                                                           Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\Essai\Essai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.79s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           1.63s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════


D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga run

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


━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  Essai.exe
     D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Bin\Debug-Windows\Essai\Essai.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Somme calculee : 5000000050000000

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (0.36s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Somme calculee : 5000000050000000

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D EXECUTION  —  termine normalement  (0.65s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

**Bilan :** le mode `Release`est plus rapide que le mode `Debug`. Mais dans un rapport je mettrais les performance pour la version release car c'est elle qui s'execute coté utilisateur.