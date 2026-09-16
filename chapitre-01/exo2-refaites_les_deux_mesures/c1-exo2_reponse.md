# ANI-4067 — Chapitre 01 : Exercice 2

> **Énoncé**  
> Le chapitre donne deux tableaux de nombres : la taille des six backends du moteur, et le coût d'un triangle par API.

>Le premier se vérifie en une commande, dans le dépôt. Faites-le, et rendez la sortie.

>Le second ne se vérifie pas dans le dépôt : il vient d'ailleurs. Dites d'où il pourrait venir, et comment vous le vérifieriez si vous en aviez le temps. 
>  


---

### Informations
* **Date :** `2026-09-16`
* **Auteur :** Kamdem

---

## Partie 1 : Taille des backends

**Commande exécutée dans le dépôt :**
```cmd
powershell -Command "Get-ChildItem -Directory | ForEach-Object { $lines = (Get-ChildItem -Path$_.FullName -Recurse -Include *.cpp,*.h,*.hpp,*.c -ErrorAction SilentlyContinue | Get-Content | Measure-Object -Line).Lines; [PSCustomObject]@{ Backend = $_.Name; Lignes =$lines } } | Format-Table -AutoSize"
```

**Resultat obtenu :**

```text
Backend           Lignes
-------           ------
DirectX11           2552
DirectX12           4582
Metal                323
Opengl              4106
Vulkan              3853
Software            6835
```

---
## Partie 2 : Coût du Triangle

**-> Provennence du tableau:**

```text
Ces seconds resultats proviennent probablement des Organisme detentrice d'entreprise de conception de de carte Graphique à l'instar de NVIDIA / AMD ou des test Benchmark de ces differentes API.
```
**-> Methodologie:**
```text

 ```
