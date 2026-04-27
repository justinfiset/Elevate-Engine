//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0

#ifndef CATCH_USER_CONFIG_HPP_INCLUDED
#define CATCH_USER_CONFIG_HPP_INCLUDED

// ------
// Overridable compilation flags
// ------

// Activation des fonctionnalités C++17/20 pour les compilateurs modernes (VS, GCC, Clang)
#define CATCH_CONFIG_CPP17_BYTE
#define CATCH_CONFIG_CPP17_OPTIONAL
#define CATCH_CONFIG_CPP17_STRING_VIEW
#define CATCH_CONFIG_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_CPP17_VARIANT

// --- GESTION MULTIPLATEFORME DES COULEURS ---
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    // On est sur Windows : on utilise l'API console Win32
    #define CATCH_CONFIG_COLOUR_WIN32
#else
    // On est sur Linux, macOS ou Emscripten : 
    // On utilise les codes ANSI standard pour les couleurs dans le terminal
    #define CATCH_CONFIG_COLOUR_ANSI
#endif

// Support du compteur (nécessaire pour certaines macros d'assertions)
#define CATCH_CONFIG_COUNTER

// ------
// Simple toggle defines
// ------

// Ces options restent désactivées par défaut pour une compatibilité maximale
// #define CATCH_CONFIG_FAST_COMPILE
// #define CATCH_CONFIG_DISABLE_EXCEPTIONS

// ------
// "Variable" defines, these have actual values
// ------

// Valeurs par défaut pour le moteur Elevate
#define CATCH_CONFIG_DEFAULT_REPORTER "console"
#define CATCH_CONFIG_CONSOLE_WIDTH 80

#endif // CATCH_USER_CONFIG_HPP_INCLUDED