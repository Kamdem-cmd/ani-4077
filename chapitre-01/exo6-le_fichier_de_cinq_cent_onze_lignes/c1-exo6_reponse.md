# ANI-4077 — Chapitre 01 : Exercice 6

> **Énoncé**  
>
>
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

# Suivi d'apprentissage Interface `NkIDevice` (Point de référence : Chapitre 20)

---

## 1. Bilan de l'interface `NkIDevice`

- **Nombre total de méthodes :** **89** *(90 si l'on inclut `NkMappedMemory::IsValid()`)*

### Répartition des méthodes par catégorie

* **Cycle de vie & Infos (8) :** `~NkIDevice`, `Initialize`, `Shutdown`, `IsValid`, `GetApi`, `IsSwapchainSrgb`, `GetCaps`, `GetContextInfo`
* **Buffers (7) :** `CreateBuffer`, `DestroyBuffer`, `WriteBuffer`, `WriteBufferAsync`, `ReadBuffer`, `MapBuffer`, `UnmapBuffer`
* **Textures (5) :** `CreateTexture`, `DestroyTexture`, `WriteTexture`, `WriteTextureRegion`, `GenerateMipmaps`
* **Samplers (2) :** `CreateSampler`, `DestroySampler`
* **Shaders (2) :** `CreateShader`, `DestroyShader`
* **Pipelines (5) :** `CreateGraphicsPipeline`, `CreateComputePipeline`, `DestroyPipeline`, `SavePipelineCache`, `LoadPipelineCache`
* **Render Passes & Framebuffers (11) :** `CreateRenderPass`, `DestroyRenderPass`, `CreateFramebuffer`, `DestroyFramebuffer`, `GetFramebufferRenderPass`, `GetSwapchainFramebuffer`, `GetSwapchainRenderPass`, `GetSwapchainFormat`, `GetSwapchainDepthFormat`, `GetSwapchainWidth`, `GetSwapchainHeight`
* **Descriptor Sets (7) :** `CreateDescriptorSetLayout`, `DestroyDescriptorSetLayout`, `AllocateDescriptorSet`, `FreeDescriptorSet`, `UpdateDescriptorSets`, `BindUniformBuffer`, `BindTextureSampler`
* **Command Buffers (2) :** `CreateCommandBuffer`, `DestroyCommandBuffer`
* **Soumission & Synchronisation (8) :** `Submit`, `SubmitAndPresent`, `CreateFence`, `DestroyFence`, `WaitFence`, `IsFenceSignaled`, `ResetFence`, `WaitIdle`
* **Gestion des Frames (5) :** `BeginFrame`, `EndFrame`, `GetFrameIndex`, `GetMaxFramesInFlight`, `GetFrameNumber`
* **Resize & Surface (2) :** `OnResize`, `RecreateSurface`
* **Queries GPU (4) :** `BeginTimestampQuery`, `EndTimestampQuery`, `GetTimestampResults`, `GetTimestampPeriodNs`
* **Accès Natif (4) :** `GetNativeDevice`, `GetNativeCommandQueue`, `GetNativePhysicalDevice`, `DestroyGpuSemaphore`
* **Statistiques & Débogage (5) :** `GetLastFrameStats`, `ResetFrameStats`, `SetDebugName` (Buffer), `SetDebugName` (Texture), `SetDebugName` (Pipeline)
* **Swapchain (2) :** `CreateSwapchain`, `DestroySwapchain`
* **Semaphores & Queues (5) :** `CreateGpuSemaphore`, `DestroySemaphore`, `HasDedicatedComputeQueue`, `SubmitOnQueue`, `SubmitGraphics`
* **Bindless Heap (5) :** `CreateBindlessHeap`, `DestroyBindlessHeap`, `WriteBindlessTexture`, `WriteBindlessBuffer`, `BindBindlessHeap`

---

## 2. Tableau comparatif d'évolution

| Indicateur | Valeur Actuelle | Valeur au Chapitre 20 | Écart |
| :--- | :---: | :---: | :---: |
| **Nombre de méthodes** | **89** | | |
| **Notions / Mots INCONNUS** | `[ À compléter ]` | | |
| **Notions / Mots RECONNUS** | `[ À compléter ]` | | |

---

## 3. Glossaire & Suivi des concepts clés

### Notions maîtrisées / reconnues
- [ ] *Exemple : Swapchain*
- [ ] *Exemple : Framebuffer*

### Notions à approfondir d'ici le Chapitre 20
- [ ] *Exemple : Bindless Heap*
- [ ] *Exemple : Timestamp Queries*
- [ ] *Exemple : Conservative Rasterization*
- [ ] *Exemple : Fences vs Semaphores*