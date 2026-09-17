# ANI-4077 — Chapitre 01 : Demo3

> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---
## 1. Décomposition des rôles et correspondance avec le Pipeline GPU

| Étudiant | Action devant la classe | Étape du Pipeline Graphique | Rôle exact dans le GPU |
| :--- | :--- | :--- | :--- |
| **Étudiant 1** | Déplace 3 points sur le tableau. | **Vertex Shader** *(Traitement des sommets)* | Applique les transformations géométriques (Model, View, Projection) pour positionner les sommets dans l'espace écran. |
| **Étudiant 2** | Identifie et colorie la surface/les cases comprises entre les 3 points. | **Rasterizer** *(Rastérisation)* | Détermine quels pixels/fragments couvrent le triangle géométrique (*Primitive Assembly* & interpolation des coordonnées). |
| **Étudiant 3** | Choisit la couleur finale de chaque case. | **Fragment / Pixel Shader** *(Shading)* | Calcule la couleur de chaque pixel (éclairage PBR, échantillonnage de textures, ombrage et matériaux). |

## 2. Analyse du modèle mental fixé par l'exercice

Cette démonstration fixe trois concepts fondamentaux de la synthèse d'image temps réel :

* **Séquentialité et découplage :** La géométrie (*Vertex*), la discrétisation spatiale (*Rasterization*) et l'apparence surfacique (*Fragment/Pixel*) sont trois étapes distinctes exécutées dans un ordre immuable.
* **Asymétrie de la charge de travail (Parallélisme GPU) :** 
  * L'Étudiant 1 ne traite que **3 données** (les sommets).
  * L'Étudiant 2 et 3 traitent potentiellement **des milliers de pixels** pour ce même triangle.
  * *Conclusion :* Le GPU consacre la majorité de sa puissance de calcul massivement parallèle au *Pixel Shading*.
* **Passage du continu au discret :** La géométrie vectorielle continue (3 points virtuels) est projetée et découpée sur une grille fixe et finie (les pixels du tableau ou de l'écran).

---

## 3. Lien avec l'architecture du moteur (`NkIDevice`)

Ce jeu d'acteur préfigure directement la configuration des objets d'état de pipeline (*PSO - Pipeline State Object*) manipulés dans l'architecture RHI :

* **`NkVertexInputLayout` & `NkVertexShader`** $\rightarrow$ Code exécuté par l'Étudiant 1.
* **`NkRasterizerState`** *(Culling, Wireframe vs Solid)* $\rightarrow$ Règles imposées à l'Étudiant 2.
* **`NkPixelShader` & `NkBlendState`** $\rightarrow$ Code et opérations d'écriture exécutés par l'Étudiant 3.