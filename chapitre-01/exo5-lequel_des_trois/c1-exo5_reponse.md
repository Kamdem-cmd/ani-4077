# ANI-4067 — Chapitre 01 : Exercice 5

> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---


> **Question de référence :** Pour chaque effet graphique, transforme-t-il des points, choisit-il des pixels, ou les colore-t-il ?

---

## Réponses aux 5 effets

1. **Une ombre portée :**  
   Elle demande **les trois** : elle transforme les points du modèle pour projeter sa géométrie dans une *shadow map*, choisit les pixels masqués en comparant leur profondeur, et les colore en les assombrissant.

2. **Un reflet dans une flaque :**  
   Il demande **les trois** : il transforme les points de la scène selon le plan de réflexion de la caméra, choisit les pixels situés à la surface de la flaque, et les colore en mélangeant l'image reflétée avec la couleur de l'eau.

3. **Un flou de mouvement (*Motion Blur*) :**  
   Il choisit les pixels voisins situés le long d'un vecteur de vitesse (calculé d'après la transformation des points entre deux images) et les colore en les étalant et en les mélangeant.

4. **Un contour dessiné autour d'un personnage (*Outline*) :**  
   Il demande **les trois** : il transforme les points pour extruder le modèle le long de ses normales, choisit les pixels qui dépassent la silhouette du personnage, et les colore avec la teinte du trait de contour.

5. **De l'herbe qui ondule :**  
   Elle demande **les trois** : elle transforme dynamiquement les points de la géométrie avec une fonction de vent dans le *vertex shader*, choisit les pixels occupés par les brins lors de la rastérisation, et les colore selon leur éclairage et leur matériau.