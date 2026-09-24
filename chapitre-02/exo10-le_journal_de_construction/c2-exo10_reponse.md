# Journal de Construction — NKRHI / MonRHI

---

### Protocole de Saisie
Pour chaque échec de compilation ou d'édition de liens rencontré durant le développement, consigner obligatoirement :
1. **Le message exact** renvoyé par le compilateur ou le lieur.
2. **La cause réelle** identifiée après analyse.
3. **Le temps de résolution** écoulé avant d'obtenir un build valide.

---

## Tableau de Suivi des Erreurs

| ID | Date | Message Exact du Build / Linker | Cause Réelle | Temps de Résolution |
| :---: | :---: | :--- | :--- | :---: |
| **01** | `2026-09-24` | `error: use of undeclared identifier 'MonRhi'` | Déclaration du `namespace MonRhi` absente du fichier d'en-tête `src/MonRhi/MonRhi.h`. | **10 min** |
| **02** | `2026-09-24` | `Projects Built: 0/0` *(Aucune erreur levée)* | Directive `with workspace(...)` commentée dans `.jenga` : les projets déclarés n'étaient reliés à aucun graphe de build. | **25 min** |
| **03** | `2026-09-24` | `undefined reference to 'MonUtil::...'` | Ordre des dépendances inversé (`links(["MonUtil", "MonRhi"])`) avec le lieur GNU `ld` (analyse mono-passe de gauche à droite). | **40 min** |
| **04** | `2026-09-24` | `fatal error: MonRhi/MonRhi.h: No such file or directory` | Chemin `src` manquant dans la directive `includedirs([...])` du projet client. | **5 min** |

---

## Modèle d'Entrée à Copier-Coller (Saisie Rapide)

```markdown
### [Erreur #00X] — AAAA-MM-JJ
* **Message exact :**
```text


  Cause réelle : <Explication de du l'origine problème technique>

Temps de résolution : <Nombre / de heures minutes passées>


---

## Bilan et Analyse Métier (Perspective Chapitre 13)

> Ce fichier sera réanalysé au Chapitre 13 lorsque la bibliothèque comportera ses 6 modules et ses 2 backends graphiques complets.
>
> La répétition de certaines erreurs (oubli systématique d'includes, mauvaise gestion de l'ordre des `links`, variables d'environnement non propagées) permettra d'identifier les biais récurrents lors de la phase de configuration de la chaîne de compilation.