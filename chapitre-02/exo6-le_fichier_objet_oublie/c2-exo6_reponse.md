# ANI-4067 — Chapitre 02 : Exercice 6

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
# Le fichier Objet oublier


* **Contenu du fichier** `unitile.cpp`

```cpp
#include <iostream>

void FonctionInutile() {
    std::cout << "Cette fonction ne sera jamais appelee." << std::endl;
}
```

* **tableau resutat**

| Fichier Cible | Emplacement dans le Build | Taille AVANT | Taille APRÈS | Variation |
| :--- | :--- | :---: | :---: | :---: |
| **MonRhi.lib** | `Build/Lib/Debug-Windows/` | 15 Ko | 29 Ko | +14 Ko (Augmentation) |
| **Essai.exe** | `Build/Bin/Debug-Windows/Essai/` | 154 Ko | 154 Ko | 0 Ko (Aucun changement) |

## Conclusion

**Un fichier en plus même inutile impacte sur la taille finale d'un lib.**