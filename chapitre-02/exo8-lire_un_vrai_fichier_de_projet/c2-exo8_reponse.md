# Lecture et Analyse de `NKRHI.jenga`

---

## 1. Dépendances de base (avant conditions)

Le module déclare **12 dépendances de base** inconditionnelles dans la liste `rhiDeps` :

1. `NKMath`
2. `NKTime`
3. `NKLogger`
4. `NKEvent`
5. `NKWindow`
6. `NKContainers`
7. `NKMemory`
8. `NKCore`
9. `NKPlatform`
10. `NKThreading`
11. `NKFileSystem`
12. `NKSL`

---

## 2. Nombre de conditions et sur quoi elles portent

Le script contient **17 blocs de filtres globaux (`filter`)** et **15+ instructions conditionnelles (`if`)**. Elles portent sur 4 axes principaux :

1. **Le système d'exploitation / la plateforme (`system:`) :**
   * Windows, UWP, Linux, macOS, iOS, Android, HarmonyOS, Web (Emscripten), Xbox Series / Xbox One.
2. **Les options de fenêtrage et de runtime (`options:`) :**
   * `windows-runtime=uwp`, `linux-backend=xlib|xcb|wayland|headless`.
3. **L'activation des bibliothèques/backends graphiques (`if`) :**
   * `WANT_VULKAN`, `USE_NKGLAD`, `USE_NKGLSLANG`, `USE_NKSPIRVCROSS`.
4. **La configuration de compilation (`config:`) :**
   * `Debug` vs `Release`.

---

## 3. Définitions préprocesseur (`defines`)

Au total, **25 defines distincts** sont posés à travers le fichier (en plus du tableau importé `WAYLAND_DEFINES`).

### A. Defines dépendant de la machine / plateforme (15) :
* **Windows / UWP :** `WIN32_LEAN_AND_MEAN`, `NK_RHI_DX11_ENABLED`, `NK_RHI_DX12_ENABLED`, `NKENTSEU_PLATFORM_UWP`
* **Linux (Fenêtrage & Vulkan Surface) :** `NKENTSEU_FORCE_WINDOWING_XLIB_ONLY`, `VK_USE_PLATFORM_XLIB_KHR`, `NKENTSEU_FORCE_WINDOWING_XCB_ONLY`, `VK_USE_PLATFORM_XCB_KHR`, `WAYLAND_DEFINES`, `VK_USE_PLATFORM_WAYLAND_KHR`, `NKENTSEU_FORCE_WINDOWING_NOOP_ONLY`
* **macOS / iOS :** `NK_RHI_METAL_ENABLED`
* **Android / HarmonyOS / Web :** `NK_OPENGL_ES`, `VK_USE_PLATFORM_ANDROID_KHR`, `VK_USE_PLATFORM_OHOS_KHR`

### B. Defines de configuration logicielle & build (10) :
* **Backends & Outils :** `NKRENDERER_USE_NKGLAD`, `NKENTSEU_ENABLE_VULKAN_BACKEND`, `NK_RHI_VK_ENABLED`, `NK_RHI_GLSLANG_ENABLED`, `ENABLE_HLSL`, `ENABLE_OPT=0`, `NK_RHI_SPIRVCROSS_ENABLED`
* **Modes de compilation :** `_DEBUG`, `DEBUG`, `NDEBUG`