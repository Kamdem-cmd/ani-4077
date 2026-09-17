# ANI-4077 — Chapitre 01 : Exercice 4

> **Énoncé**  

>Le chapitre donne la liste de ce que vous recevez et de ce que vous écrivez.

>Pour chacun des cinq éléments reçus, répondez à une question : que faudrait-il écrire si vous ne le receviez pas ?

>Donnez un ordre de grandeur en lignes, avec la façon dont vous l'estimez. Une estimation justifiée vaut mieux qu'un chiffre juste sorti de nulle part.


---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

## 1. Synthèse des éléments reçus vs code à écrire

| Élément reçu | Ce qu'il faudrait écrire sans l'élément | Estimation (lignes) | Justification de l'estimation |
| :--- | :--- | :---: | :--- |
| **1. Fenêtre & Contexte OS** | Boucle Win32 (`CreateWindowEx`, `PeekMessage`), gestion DPI et événements. | **150 - 300** | Fichier source d'initialisation Win32 standard en C++. |
| **2. Backend RHI** | Instance API, sélection GPU, Logical Device, Swapchain, Sync (Semaphores/Fences). | **1 500 - 4 000** | Basé sur les métriques statiques du projet NKRHI (Vulkan = ~3850 L, DX12 = ~4580 L). |
| **3. Mathématiques 3D** | Classes `Vec3`, `Mat4`, produit matriciel, transformations, projections (Perspective/Ortho). | **500 - 800** | Taille d'une bibliothèque mathématique 3D minimale (type GLM simplifiée). |
| **4. Mémoire GPU & Buffers** | Allocations mémoire GPU, alignement, staging buffers pour transferts CPU/GPU. | **600 - 1 200** | Système de gestion de mémoire GPU simplifié avec sous-allocation. |
| **5. Chargeur de Shaders** | Entrées/Sorties binaires, chargement des fichiers SPIR-V/DXBC, création des modules. | **250 - 500** | Gestionnaire de fichiers C++ et wrappers d'API pour les Shader Modules. |

---

## 2. Conclusion

Sans les abstractions fournies par le framework, l'affichage du premier triangle nécessiterait entre **3 000 et 7 000 lignes de code d'infrastructure** avant même d'écrire la moindre logique applicative.