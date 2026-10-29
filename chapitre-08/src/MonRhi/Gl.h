// =============================================================================
//  Étape 04 — le seul endroit du projet qui choisit entre OpenGL et OpenGL ES.
//
//  glad/gl.h déclare OpenGL « de bureau ». glad/gles2.h déclare OpenGL ES.
//  Les deux déclarent glClear, glViewport, glClearColor, et les inclure
//  ensemble donne une centaine d'erreurs de redéfinition.
//
//  Le choix se fait donc ICI, une fois, et aucun autre fichier n'a le droit
//  d'inclure glad directement.
//
//  UN AVEU, ET IL EST IMPORTANT : ce fichier ne devrait pas exister.
//
//  Il existe parce qu'au chapitre 5, votre programme parle encore OpenGL
//  directement : il appelle glClearColor et glClear lui-même. Tant que c'est
//  le cas, il lui faut les prototypes, donc il lui faut cet en-tête.
//
//  Au chapitre 13, cet en-tête disparaîtra du programme et restera dans la
//  bibliothèque. Ce jour-là, le code de la scène n'écrira plus une seule fois
//  le mot « gl », et vous pourrez mesurer le chemin parcouru en comptant les
//  fichiers qui incluent celui-ci : ils sont deux aujourd'hui, il en restera
//  un.
// =============================================================================
#pragma once

#include "NKPlatform/NkPlatformDetect.h"

#if defined(NKENTSEU_PLATFORM_WINDOWS)
#include <glad/gl.h>

#elif defined(NKENTSEU_WINDOWING_XLIB)
#include <glad/gl.h>

#elif defined(NKENTSEU_PLATFORM_ANDROID) || defined(NKENTSEU_PLATFORM_HARMONYOS) ||                                    \
	defined(NKENTSEU_PLATFORM_EMSCRIPTEN)
#include <glad/gles2.h>

#else
// Aucun chemin OpenGL sur ce système. Le programme compilera, et Contexte
// refusera poliment de créer quoi que ce soit.
#endif
