# ANI-4067 — Chapitre 02 : Exercice 4

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
# Espace de travail
```powershell
from Jenga import *
import sys
print("configurations :", ["Debug", "Release"], file=sys.stderr)

with workspace("MonRhiWks", location="."):
    configurations(["Debug", "Release"])

    # La bibliotheque : du code compile, sans main.
    with project("MonRhi"):
        staticlib()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/MonRhi/**.cpp"])
        includedirs(["src"])
        objdir("%{wks.location}/Build/Obj/"
                "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/"
                    "%{cfg.buildcfg}-%{cfg.system}")

    # Le programme qui s'en sert.
    with project("Essai"):
        consoleapp()
        language("C++")
        cppdialect("C++17")
        location(".")
        files(["src/Essai/**.cpp"])
        includedirs(["src"])
        libdirs(["%{wks.location}/Build/Lib/"
                    "%{cfg.buildcfg}-%{cfg.system}"])
        links(["MonRhi"])
        objdir("%{wks.location}/Build/Obj/"
                "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
        targetdir("%{wks.location}/Build/Bin/"
                    "%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
```

# Resultat de la commande `jenga info`


```powershell
╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

configurations : ['Debug', 'Release']
========================== Jenga Workspace: MonRhiWks ==========================

Location: D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI
Entry file: D:\ENSPY\AN-GAP_4\teguis\ani-4077\chapitre-02\MonRHI\MonRHI.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes:
Target Architectures:


Projects
------------------------------------------------------------
Name     Kind         Language   Test   External
================================================
MonRhi   StaticLib    C++        No     No
Essai    ConsoleApp   C++        No     No


Available Toolchains
------------------------------------------------------------
Name                 Family        Target OS   Arch     Env
===============================================================
host-clang           clang         Windows     x86_64   mingw
host-gcc             gcc           Windows     x86_64   mingw
msvc                 msvc          Windows     x86_64   msvc
clang-mingw          clang         Windows     x86_64   mingw
mingw                gcc           Windows     x86_64   mingw
clang-cross-linux    clang         Linux       x86_64   gnu
android-ndk          android-ndk   Android     arm64    android
zig-linux-x86_64     clang         Linux       x86_64   gnu
zig-linux-x64        clang         Linux       x86_64   gnu
zig-windows-x86_64   clang         Windows     x86_64   mingw
zig-windows-x64      clang         Windows     x86_64   mingw
zig-macos-x86_64     clang         macOS       x86_64   gnu
zig-macos-arm64      clang         macOS       arm64    gnu
zig-ios-arm64        clang         iOS         arm64
zig-tvos-arm64       clang         tvOS        arm64
zig-watchos-arm64    clang         watchOS     arm64
zig-android-arm64    clang         Android     arm64    android
zig-web-wasm32       clang         Web         wasm32


Daemon
------------------------------------------------------------
Status: Not running
```
