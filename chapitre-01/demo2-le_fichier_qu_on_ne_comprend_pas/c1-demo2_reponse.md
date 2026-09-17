# ANI-4077 — Chapitre 01 : Demo 2
> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

# Compte-rendu d'exercice — Projection et engagement sur `NkIDevice.h`

---

## 1. Protocole pédagogique

* **Fichier projeté :** `NkIDevice.h` (Interface Rationale & Abstraction Hardware RHI)
* **Modalité :** Défilement silencieux de 30 secondes
* **Engagement solennel :**

> **« À la fin de ce cours, vous saurez lire ce fichier. »**

---

## 2. Rôle stratégique de `NkIDevice.h`

Le fichier `NkIDevice.h` constitue le contrat central de la couche d'abstraction RHI (*Render Hardware Interface*). Il rassemble l'ensemble des structures de descripteurs, des allocations de ressources GPU et des signatures de méthodes virtuelles pures agnostiques de l'API hôte.

Au début du cursus, le fichier apparaît comme une structure dense et intimidante de déclarations C++. 

---

## 3. Matrice de progression (Jalon Chapitre 1 $\rightarrow$ Chapitre 20)

| Dimension | Chapitre 1 (Projection initiale) | Chapitre 20 (Jalon de clôture) |
| :--- | :--- | :--- |
| **Perception du code** | Accumulation de types complexes et de pointeurs opaques. | Architecture claire où chaque type possède une responsabilité précise. |
| **Statut de `NkIDevice`** | Spécification théorique distante. | Interface fully-implemented pilotant le backend (Vulkan / OpenGL). |
| **Objectif pédagogique** | Ancrage visuel et création d'une attente d'apprentissage. | Validation tangible de la promesse et synthèse du moteur. |

---

## 4. Portée de l'exercice

Cette action pédagogique de 5 minutes fixe le contrat d'apprentissage du semestre :

1. **Démystification méthodique :** Chaque unité du cours (buffers au Ch. 10, pipelines au Ch. 13, synchronisation au Ch. 17) donne du sens à un bloc précis de `NkIDevice.h`.
2. **Évaluation bilancielle :** La relecture finale du fichier au **Chapitre 20** sert de preuve concrète que la complexité logicielle a été assimilée et maîtrisée.