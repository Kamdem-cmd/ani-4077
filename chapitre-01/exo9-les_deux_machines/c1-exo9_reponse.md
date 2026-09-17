# ANI-4077 — Chapitre 01 : Exercice 9

> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

## 1. Caractéristiques de mon système

| Métrique Matérielle | Description | Valeur Mesurée / Théorique |
| :--- | :--- | :---: |
| **Mémoire Carte Graphique (VRAM)** | Capacité totale de la mémoire dédiée GPU | **128 Mo** |
| **Débit Théorique Bus PCIe** | Vitesse maximale du bus (CPU ↔ GPU) | **15,75 Go/s** |
| **Débit Mémoire Centrale (RAM)** | Vitesse de la mémoire système hôte | **25,6 Go/s** |


---

## 2. Calculs des temps de transfert sur le Bus PCIe

### Cas A : Transfert d'un maillage de 1 000 000 de sommets

* **Taille d'un sommet standard (Vertex) :**
  * Position (`Vec3` float) : $3 \times 4 \text{ octets} = 12 \text{ octets}$
  * Normale (`Vec3` float) : $3 \times 4 \text{ octets} = 12 \text{ octets}$
  * Coordonnées UV (`Vec2` float) : $2 \times 4 \text{ octets} = 8 \text{ octets}$
  * **Total par sommet :** $32 \text{ octets}$

* **Taille totale du maillage ($S_{\text{mesh}}$) :**
  $$S_{\text{mesh}} = 1\,000\,000 \times 32 \text{ octets} = 32\,000\,000 \text{ octets} = \mathbf{32 \text{ Mo}}$$

* **Temps de transfert théorique ($T_{\text{mesh}}$) :**
  $$T_{\text{mesh}} = \frac{S_{\text{mesh}}}{\text{Débit PCIe}} = \frac{32 \text{ Mo}}{15\,750 \text{ Mo/s}} \approx \mathbf{2{,}03 \text{ ms}}$$

---

### Cas B : Transfert d'une texture 4096 × 4096 pixels

* **Format de texture standard :** `RGBA8` ($4 \text{ octets}$ par pixel).

* **Taille totale de la texture ($S_{\text{tex}}$) :**
  $$S_{\text{tex}} = 4096 \times 4096 \times 4 \text{ octets} = 67\,108\,864 \text{ octets} \approx \mathbf{67{,}11 \text{ Mo}}$$

* **Temps de transfert théorique ($T_{\text{tex}}$) :**
  $$T_{\text{tex}} = \frac{S_{\text{tex}}}{\text{Débit PCIe}} = \frac{67{,}11 \text{ Mo}}{15\,750 \text{ Mo/s}} \approx \mathbf{4{,}26 \text{ ms}}$$

---

## 3. Analyse théorique pour le Chapitre 8

Les durées obtenues (**~2,0 ms** pour le maillage et **~4,3 ms** pour la texture) représentent **plus de 50 % du budget temps total d'une image à 90 Hz (11,1 ms)** uniquement pour acheminer la donnée à travers le bus PCIe.

Ces résultats mettent en évidence deux piliers fondamentaux de l'architecture graphique (Chapitre 8) :

1. **Le bus PCIe est un goulet d'étranglement majeur :** Il est physiquement impossible de retransférer l'ensemble des géométries et des textures depuis la mémoire centrale (RAM CPU) vers le GPU à chaque image sans s'exposer à un effondrement du taux de rafraîchissement.
2. **Allocation statique et persistance VRAM :** Les ressources graphiques doivent être chargées **une fois pour toutes** en VRAM lors des phases d'initialisation ou de chargement de niveau. Pour les données dynamiques modifiées à chaque frame (ex: matrices d'instances, paramètres d'uniformes), le moteur doit limiter le volume transféré au strict minimum via des tampons visibles par l'hôte (*Host-Visible / Staging Buffers*).