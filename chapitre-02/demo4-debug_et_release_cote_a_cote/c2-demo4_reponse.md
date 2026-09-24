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

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           1.82s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════


D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga run --config release


━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  Essai.exe
     D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Bin\release-Windows\Essai\Essai.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Somme calculee : 5000000050000000

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D EXECUTION  —  termine normalement  (0.36s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```
* **2. En Debug :**

```powershell
D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga build --config debug

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           20.17s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════


D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI>jenga run --config debug

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  Essai.exe
     D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\Build\Bin\Debug-Windows\Essai\Essai.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Somme calculee : 5000000050000000

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D EXECUTION  —  termine normalement  (0.65s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

**Bilan :** le mode `Release`est plus rapide que le mode `Debug`. Mais dans un rapport je mettrais les performance pour le mode Debug. la version release s'execute coté utilisateur.