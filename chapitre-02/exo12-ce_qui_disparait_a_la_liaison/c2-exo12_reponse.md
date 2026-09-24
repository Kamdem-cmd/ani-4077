# ANI-4067 — Chapitre 02 : Exercice — Le Piège des Constructeurs Globaux et des Bibliothèques Statiques

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem
* **Cadre :** Étude du comportement du lieur face aux effets de bord (*static registration*)

---

## 1. Reproduction de l'Expérience (Le Piège)

Création d'un fichier source `EnregistrementAuto.cpp` au sein du projet `MonRhi`. Ce fichier contient une variable globale dont le constructeur doit exécuter un code d'initialisation au démarrage de l'application.

### Code de `src/MonRhi/EnregistrementAuto.cpp`

```cpp
#include <iostream>

namespace MonRhi {
    struct EnregistreurAuto {
        EnregistreurAuto() {
            std::cout << "[MonRhi] MODULE D'ENREGISTREMENT AUTOMATIQUE INITIALISÉ !" << std::endl;
        }
    };

    // Variable globale : son constructeur doit s'exécuter avant d'entrer dans main()
    static EnregistreurAuto g_autoRegister;
}

