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

# Compte-rendu d'exercice — Analyse des Capacités Matérielles Avancées & Périmètre Pédagogique

---

## 1. Données Matérielles du Système

* **GPU Dédié / Intégré :** Intel(R) HD Graphics 620 (Architecture Kaby Lake GT2)
* **Mémoire :** 128 Mo VRAM dédiée (allouée dynamiquement sur la RAM DDR4 système)
* **Support d'API :** OpenGL 4.6, Vulkan, DirectX 12 (12_1), DirectCompute, OpenCL
* **Fonctionnalités Matérielles Absentéistes :** Hardware Ray Tracing, DirectML / Unités IA

---

## 2. Analyse des Trois Pistes d'Architecture

### A. Le Calcul Général (GPGPU / Compute Shaders)

| Question | Analyse Technique |
| :--- | :--- |
| **Existe-t-il sur la carte ?** | **Oui.** Le GPU-Z confirme le support de **DirectCompute** et **OpenCL**, ainsi que les pipelines de calcul sous OpenGL 4.6 et Vulkan (24 *Execution Units* unifiées). |
| **Utilité dans un moteur réel ?** | Exécution d'algorithmes généraux hors-pipeline classique : simulations de fluides/particules, *GPU-driven rendering* (frustum & occlusion culling sur GPU), et effets de post-traitement complexes (*Clustered Shading*, *Depth of Field*). |
| **Pourquoi le livre s'en passe ?** | Le *Compute Shader* introduit des problématiques complexes de synchronisation mémoire (*barriers*, opérations atomiques, taille de *workgroups*) qui masquent les concepts fondamentaux du pipeline de rendu (*Vertex $\rightarrow$ Rasterizer $\rightarrow$ Pixel*). |

---

### B. Le Lancer de Rayons Matériel (Hardware Ray Tracing)

| Question | Analyse Technique |
| :--- | :--- |
| **Existe-t-il sur la carte ?** | **Non.** La case **Ray Tracing** est décochée sous GPU-Z. L'architecture ne possède aucun composant matériel dédié à la traversée de structures hiérarchiques BVH (*Bounding Volume Hierarchy*). |
| **Utilité dans un moteur réel ?** | Calcul analytique exact de phénomènes optiques physiques : éclairage global (*Path Tracing* temps réel), réflexions et réfractions hors champ de la caméra, et ombres douces physiques. |
| **Pourquoi le livre s'en passe ?** | La rastérisation demeure le paradigme universel le plus performant du rendu temps réel. Le *Ray Tracing* est une couche additive qui nécessite d'abord une maîtrise absolue des transformations géométriques et des pipelines d'ombrage. |

---

### C. Les Unités Dédiées à l'Apprentissage Automatique (AI Cores / DirectML)

| Question | Analyse Technique |
| :--- | :--- |
| **Existe-t-il sur la carte ?** | **Non.** La technologie **DirectML** est indiquée comme non prise en charge. Aucune matrice de calcul tensoriel (type *Tensor Cores* ou *XMX*) n'est intégrée à la puce. |
| **Utilité dans un moteur réel ?** | Suréchantillonnage temporel intelligent (DLSS, XeSS), dénoyautage de rayons (*Ray Denoising*), et synthèse d'animations ou de comportements par réseaux de neurones. |
| **Pourquoi le livre s'en passe ?** | *(Analyse détaillée ci-dessous)* |

#### Analyse approfondie du rejet des unités IA dans le cursus (`NkIDevice`)

1. **Calcul Analytique vs Inférence Statistique :** L'IA graphique fonctionne par prédiction statistique (deviner la valeur d'un pixel). Un moteur de jeu d'apprentissage doit enseigner le calcul analytique et déterministe fondé sur la physique de la lumière (équation du rendu de Kajiya, BRDF microfacettes) avant d'aborder des raccourcis d'inférence.
2. **Architecture Ouverte vs Boîte Noire Propriétaire :** Les bibliothèques de super-échantillonnage IA (DLSS, XeSS) sont fournies sous forme de binaires fermés. Les intégrer d'emblée masquerait la gestion bas niveau des barrières de synchronisation, de l'organisation mémoire et du *Framebuffer*.
3. **Agnostisme et Souveraineté de la couche RHI :** Une couche d'abstraction comme `NkIDevice` doit extraire le dénominateur commun de tout matériel (du processeur intégré Intel HD 620 à la carte graphique de station de travail). S'affranchir des unités spécialisées garantit que le moteur fonctionne par principes premiers (*first principles*) sur n'importe quel GPU.

---

## 3. Synthèse Comparative

```markdown
| Composant Avancé | Présent sur Intel HD 620 ? | Impact sur la Couche RHI (`NkIDevice`) |
| :--- | :---: | :--- |
| **GPGPU / Compute** | **OUI** | Extension future via des files d'attente de calcul dédiées (`NkComputePipeline`). |
| **Ray Tracing** | **NON** | Nécessiterait une abstraction complexe de structures d'accélération (BVH). |
| **Unités IA / DirectML** | **NON** | Hors périmètre : le moteur privilégie la rigueur physique et la portabilité. |