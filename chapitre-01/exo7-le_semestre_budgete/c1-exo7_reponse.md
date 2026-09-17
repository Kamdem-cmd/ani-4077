# ANI-4077 — Chapitre 01 : Exercice 7

> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---
# Exercice : Estimation de la charge d'apprentissage — Vulkan vs OpenGL

---

## 1. Méthodologie d'estimation

Pour évaluer si l'affirmation (*"apprendre Vulkan consommerait le semestre"*) est fondée ou exagérée, la méthode s'appuie sur :
1. **Un tutoriel de référence reconnu par la communauté** pour chaque API.
2. **Le décompte des chapitres requis** pour passer de zéro ("Hello World") à l'affichage du **premier triangle** (rendu de géométrie de base).
3. **Le volume de code C++ minimum** généré.
4. **L'estimation temporelle** découpée en :
   * **Lecture et assimilation théorique** (compréhension des concepts d'architecture GPU).
   * **Mise en œuvre pratique et débogage** (configuration, boilerplate, gestion de la mémoire/synchronisation).

---

## 2. Analyse pour Vulkan

* **Tutoriel de référence :** [vulkan-tutorial.com](https://vulkan-tutorial.com/) (Alexander Overvoorde)

### Décompte des chapitres jusqu'au premier triangle : **18 chapitres**
1. *Overview*
2. *Development environment*
3. *Instance*
4. *Validation layers*
5. *Physical devices and queue families*
6. *Logical device and queues*
7. *Window surface*
8. *Swap chain*
9. *Image views*
10. *Graphics pipeline basics*
11. *Shader modules*
12. *Fixed functions*
13. *Render passes*
14. *Conclusion (Pipeline overview)*
15. *Framebuffers*
16. *Command pools*
17. *Command buffers*
18. *Rendering and presentation (Frames in flight)*

### Métriques & Estimation
* **Volume de code C++ :** **~1 000 à 1 200 lignes** de code explicite sans abstraction.
* **Concepts explicites à gérer :** Allocations mémoire GPU, pipelines d'état immutables, barrières de synchronisation, *Swapchain*, *Render Passes*, *Command Buffers*, compilation SPIR-V, *Validation Layers*.
* **Temps de lecture et d'assimilation théorique :** ~20h à 25h
* **Temps de codage, configuration et débogage :** ~30h à 40h
* **Temps total pour le 1er triangle :** **~50h à 65h**

> **Impact à l'échelle d'un semestre :** Obtenir un simple triangle ne signifie pas "savoir utiliser l'API". Pour construire un petit moteur fonctionnel (chargement de maillages, *Uniform Buffers*, *Bindless descriptors*, ombre, multithreading), il faut compter **entre 150h et 250h d'effort**, ce qui équivaut au Semestre voir plus.

---

## 3. Analyse pour OpenGL

* **Tutoriel de référence :** [learnopengl.com](https://learnopengl.com/) (Joey de Vries)

### Décompte des chapitres jusqu'au premier triangle : **4 chapitres**
1. *OpenGL* (Introduction théorique)
2. *Creating a window* (GLFW / Glad)
3. *Hello Window*
4. *Hello Triangle* (VBO, VAO, EBO, Shaders GLSL basiques)

### Métriques & Estimation
* **Volume de code C++ :** **~120 à 150 lignes** de code.
* **Concepts à gérer :** Context GL, tampons (*Buffer Objects*), *Vertex Array Objects* (VAO), compilation GLSL à la volée, machine à états globale.
* **Temps de lecture et d'assimilation théorique :** ~2h à 3h
* **Temps de codage et mise en œuvre :** ~3h à 5h
* **Temps total pour le 1er triangle :** **~5h à 8h**

> **Impact à l'échelle d'un semestre :** En OpenGL, un étudiant atteint le niveau "moteur 3D fonctionnel avec éclairage, textures et caméra" en 30h à 50h de travail.

---

## 4. Tableau comparatif

| Critère | OpenGL (LearnOpenGL) | Vulkan (Vulkan-Tutorial) | Facteur multiplicateur |
| :--- | :---: | :---: | :---: |
| **Chapitres pour 1 triangle** | **4** | **18** | x 4.5 |
| **Lignes de code (C++)** | **~130** | **~1 100** | x 8.5 |
| **Temps 1er triangle** | **~6h** | **~55h** | x 9 |
| **Complexité architecturale** | Faible (Machine à états) | Extrême (Gestion explicite du GPU) | — |
| **Temps maîtrise globale** | ~40h à 60h | ~180h à 250h | x 4 à 5 |

---

## 5. Conclusion & Verdict : Le chapitre exagère-t-il ?

**Non, le chapitre n'exagère pas.** L'ordre de grandeur est scientifiquement et pédagogiquement correct.

1. **Rapport d'effort de 1 à 9 pour le premier résultat :** Là où OpenGL masque la synchronisation, les allocations mémoire, les allocations de formats de surface et la préparation des commandes, Vulkan exige que le développeur écrive le driver applicatif lui-même.
2. **Charge cognitive élevée :** Enseigner Vulkan à des étudiants implique d'enseigner simultanément l'architecture matérielle moderne des GPU (architecture Tile-based vs Immediate, barrières mémoire, queues de soumission async).
3. **Poids pédagogique :** Si un cours d'un semestre représente environ 120h à 150h de travail total (cours + TP + projet), choisir Vulkan signifie que **50% du semestre est consommé avant même de commencer à faire de la synthèse d'image ou du gameplay**.

**Verdict :** L'affirmation est **justifiée**. Vulkan requiert une charge de travail sur un semestre complet là où OpenGL permet de traiter la 3D dès les premières semaines.