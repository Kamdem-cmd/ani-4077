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


# Sommaire du Tome Vulkan — Architecture & Implémentation (`NkIDevice`)

---

## 1. Répartition Générale des 23 Chapitres

Le tome dédié à l'implémentation Vulkan conserve la structure théorique et agnostique du livre original. Conformément au modèle pédagogique NKRHI :
* **14 chapitres restent strictement inchangés** (fondements mathématiques, optique, architecture système, patterns orientés moteur).
* **9 chapitres sont modifiés** pour substituer la gestion d'état implicite d'OpenGL par l'explicitation matérielle imposée par Vulkan.

---

## 2. Table des Matières Globale

| N° | Titre du Chapitre | Statut | Modèle / Portée |
| :-: | :--- | :-: | :--- |
| **01** | Philosophie du Moteur & Modèle Mental Pipeline | **Inchangé** | Pédagogie et rôle des 3 étapes |
| **02** | Mathématiques 3D, Vecteurs & Transformations | **Inchangé** | Algèbre linéaire agnostique |
| **03** | Métriques Temporelles & Profilage `std::chrono` | **Inchangé** | Analyse du budget d'image applicatif |
| **04** | Optique, Stéréoscopie & Écart Interpupillaire | **Inchangé** | Géométrie de la vision VR |
| **05** | Conception Naïve de l'Interface (`NkIDevice.h`) | **Inchangé** | Définition du contrat RHI |
| **06** | Architecture Système & Débits du Bus PCIe | **Inchangé** | Contraintes de transfert CPU/GPU |
| **07** | **Instance, Physical Device & Queues Vulkan** | **MODIFIÉ** | *Implémentation Backend* |
| **08** | **Gestion Manuelle de la Mémoire GPU & VMA** | **MODIFIÉ** | *Implémentation Backend* |
| **09** | **Command Pools & Enregistrement des Orders** | **MODIFIÉ** | *Implémentation Backend* |
| **10** | Layouts de Sommets & Buffers Géométriques | **Inchangé** | Concept de formats d'attributs |
| **11** | **Compilation Shaders & Bytecode SPIR-V** | **MODIFIÉ** | *Implémentation Backend* |
| **12** | Graphique Scène & Render Graph High-Level | **Inchangé** | Ordonnancement haut niveau |
| **13** | **Objets d'État de Pipeline Immutables (PSO)** | **MODIFIÉ** | *Implémentation Backend* |
| **14** | **Binding de Ressources & Descriptor Sets** | **MODIFIÉ** | *Implémentation Backend* |
| **15** | Textures, Formats & Filtrage d'Échantillonnage | **Inchangé** | Théorie du traitement d'image |
| **16** | **Gestion de l'Affichage & Vulkan Swapchain** | **MODIFIÉ** | *Implémentation Backend* |
| **17** | **Synchronisation Matérielle : Fences & Barriers** | **MODIFIÉ** | *Implémentation Backend* |
| **18** | Matériaux PBR & Modèles d'Éclairage | **Inchangé** | Formules mathématiques physiques |
| **19** | Techniques d'Ombrage & Shadow Mapping | **Inchangé** | Algorithmes de profondeur |
| **20** | Bilan d'Architecture et Relecture de `NkIDevice.h` | **Inchangé** | Validation du contrat RHI |
| **21** | **Profilage Matériel & Queries Temporelles GPU** | **MODIFIÉ** | *Implémentation Backend* |
| **22** | Partitionnement Spatial & Culling | **Inchangé** | Structures de données (Octree/BVH) |
| **23** | Post-traitements & Intégration Finale | **Inchangé** | Chaîne d'effets plein écran |

---

## 3. Justification Détaillée des 9 Chapitres Modifiés

### Chapitre 7 : Initialisation Matérielle — Instance, Physical Device & Queues
* **Ce qui change :** Remplacement de la création de contexte implicite (GL/WGL) par l'énumération explicite des cartes graphiques physiques (`VkPhysicalDevice`), le choix des familles de files d'attente (`Queue Families`) et l'instanciation du périphérique logique (`VkDevice`).
* **Justification :** Vulkan n'a pas de contexte global actif ; il faut sélectionner manuellement les extensions matérielles et les files de soumission au démarrage.

### Chapitre 8 : Allocations Mémoire GPU — Heaps & Vulkan Memory Allocator
* **Ce qui change :** Abandon de l'allocation opaque du pilote au profit de l'allocation explicite sur les tas de mémoire (`VkDeviceMemory`), de la gestion du flag `HOST_VISIBLE` vs `DEVICE_LOCAL`, et de l'intégration de VMA (*Vulkan Memory Allocator*).
* **Justification :** Vulkan limite le nombre d'allocations système individuelles (ex. 4096 allocations max) ; le moteur doit gérer sa propre mémoire VRAM via des sous-allocations.

### Chapitre 9 : Enregistrement des Commandes — Command Pools & Command Buffers
* **Ce qui change :** Remplacement des appels d'exécution immédiate par la création de `VkCommandPool`, l'allocation de `VkCommandBuffer` et l'enregistrement asynchrone des ordres de rendu.
* **Justification :** Les commandes Vulkan ne s'exécutent pas lors de l'appel de fonction, mais sont enregistrées dans des tampons réutilisables avant d'être soumises à une `VkQueue`.

### Chapitre 11 : Pipeline d'Ombrage — Compilation GLSL vers SPIR-V
* **Ce qui change :** Remplacement du compilateur de code source en chaîne de caractères texte à la volée par la compilation hors-ligne ou au démarrage du code GLSL/HLSL en bytecode intermédiaire **SPIR-V** (`VkShaderModule`).
* **Justification :** Vulkan refuse d'ingérer du code source texte brut pour supprimer le coût d'analyse syntaxique du pilote hôte.

### Chapitre 13 : Configuration du Pipeline — Objets d'État Immutables (PSO)
* **Ce me qui change :** Abandon de la machine à états dynamique (ex: `glEnable`/`glDisable`) au profit de la création préalable d'objets `VkPipeline` monolithiques et immutables intégrant rastérisation, blending et stencil.
* **Justification :** Vulkan exige la connaissance préalable de la totalité de l'état du pipeline GPU pour valider les optimisations matérielles lors de la compilation du PSO.

### Chapitre 14 : Organisation des Données Shaders — Descriptor Sets & Layouts
* **Ce qui change :** Substitution du binding individuel d'uniformes (`glGetUniformLocation`) par la définition de tables de ressources (`VkDescriptorSetLayout`), de pools d'allocation (`VkDescriptorPool`) et de jeux de descripteurs (`VkDescriptorSet`).
* **Justification :** Vulkan regroupe les ressources (buffers, textures) en jeux d'invalidation pour minimiser les coûts de basculement de contexte CPU/GPU.

### Chapitre 16 : Moteur de Présentation — Vulkan Swapchain & Surface
* **Ce qui change :** Remplacement du simple tampon d'échange d'affichage par la gestion explicite de la chaîne de présentation (`VkSwapchainKHR`), l'acquisition des images, et la création de vues d'images (`VkImageView`).
* **Justification :** Vulkan isole l'affichage du rendu ; le moteur doit piloter manuellement la synchronisation avec l'écran et le nombre d'images en circulation dans la chaîne d'affichage.

### Chapitre 17 : Synchronisation Bas Niveau — Fences, Semaphores & Barriers
* **Ce qui change :** Intégration des primitives de synchronisation explicites : `VkFence` (CPU-GPU), `VkSemaphore` (GPU-GPU inter-queue) et `VkPipelineStageBarrier` (dépendances de mémoire et de transition de layout).
* **Justification :** Le pilote Vulkan ne gère plus les hazards mémoire ni l'ordre d'exécution ; le moteur doit spécifier manuellement quand une ressource est prête à être lue ou écrite.

### Chapitre 21 : Métrologie Matérielle — Profilage par Queries Temporelles Vulkan
* **Ce qui change :** Remplacement des requêtes temporelles de l'API par des pools de requêtes matérielles (`VkQueryPool` de type `VK_QUERY_TYPE_TIMESTAMP`) insérées directement dans les `VkCommandBuffer`.
* **Justification :** L'enregistrement asynchrone impose la lecture des horloges GPU directement au sein de la file de commandes pour mesurer les étapes exactes d'exécution sans bloquer le thread principal.