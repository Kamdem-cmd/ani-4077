# ANI-4077 — Chapitre 01 : Exercice 8

> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

# Specification Naïve de la Couche d'Abstraction Graphique (`NkIDevice`)

Document de cadrage rédigé avant l'écriture de la moindre ligne de code OpenGL, destiné à établir le modèle mental initial de notre RHI (*Render Hardware Interface*) pour réévaluation au **Chapitre 12**.

---

## 1. Ce qu'elle saura faire

La couche d'abstraction doit agir comme le chef d'orchestre neutre entre la logique applicative du moteur et le matériel graphique.

* **Gestion du cycle de vie des ressources GPU :** Créer, allouer et détruire de manière sécurisée les tampons de données (`VertexBuffer`, `IndexBuffer`, `UniformBuffer`) et les textures.
* **Encapsulation des Shaders et Pipelines :** Charger, compiler et lier des programmes d'ombrage, tout en exposant une structure d'état fixe (*Pipeline State Object*) pour le découpage géométrique, le blending et le test de profondeur.
* **Unification des ordres de rendu :** Fournir une API agnostique permettant d'enregistrer et de soumettre des commandes de dessin (*Draw Calls*) sans exposer la logique interne de l'API sous-jacente.
* **Interfaçage multi-backend :** Exposer une interface virtuelle pure (`NkIDevice`) capable de commuter de manière transparente entre un backend OpenGL et un backend Vulkan ou DirectX.

---

## 2. Ce qu'elle ignorera

Pour maintenir une responsabilité unique, la couche RHI doit rester strictement cantonnée au dialogue bas niveau avec la carte graphique.

| Domaine | Ce que l'abstraction ignore délibérément | Responsable légitime |
| :--- | :--- | :--- |
| **Graphe de Scène** | La hiérarchie des objets, les transformations spatiales (matrices Model/View/Projection) et le principe de caméra. | Moteur de jeu / Scene Graph |
| **Formats de Fichiers** | Le parsing et le décodage des fichiers assets (PNG, JPEG, OBJ, glTF, FBX). Elle ne traite que des blocs d'octets bruts (*raw byte arrays*). | Asset Pipeline / Loaders |
| **Gestion de Fenêtrage** | La création de la fenêtre de rendu, le contexte de surface natif (HWND, X11) et la capture des événements système (clavier, souris). | GLFW / SDL / OS Natif |
| **Logique Métier** | La notion de lumière, de matériau PBR, de physique ou d'intelligence artificielle. | Subsystems applicatifs supérieurs |

---

## 3. Ce qu'elle refusera

La couche d'abstraction doit imposer une discipline rigoureuse afin de prévenir les erreurs d'architecture et la dette technique.

* **Refus des appels directs à l'API hôte :** Interdiction stricte de laisser fuiter des fonctions OpenGL (`glDrawArrays`, `glBindTexture`) ou des types natifs (`GLuint`) en dehors de l'implémentation du backend.
* **Refus de l'instanciation sans descripteur :** Interdiction de créer une ressource sans fournir un descripteur explicite (`NkBufferDesc`, `NkTextureDesc`) définissant sa taille, son usage et son format.
* **Refus des changements d'état sauvages :** Refus des modifications d'état global du GPU à la volée. Toute modification doit obligatoirement passer par la soumission d'une commande structurée.
* **Refus de la gestion implicite de la mémoire :** Refus de la destruction silencieuse des ressources ; chaque allocation GPU doit suivre un cycle de vie RAII strict ou un nettoyage explicite avant la fermeture du contexte.

---

> **Note pour la relecture du Chapitre 12 :** 
> Ce document servira de témoin pour évaluer l'écart entre nos intuitions théoriques initiales et la réalité technique de la gestion d'états, du binding de ressources et des contraintes réelles imposées par le pipeline OpenGL.