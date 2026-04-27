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

// On laisse Catch2 détecter automatiquement la plupart des réglages pour Windows
// Mais on force l'utilisation des fonctionnalités C++17/20 disponibles dans VS
#define CATCH_CONFIG_CPP17_BYTE
#define CATCH_CONFIG_CPP17_OPTIONAL
#define CATCH_CONFIG_CPP17_STRING_VIEW
#define CATCH_CONFIG_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_CPP17_VARIANT

// Couleur dans la console Windows
#define CATCH_CONFIG_COLOUR_WIN32

// Support du compteur (pour les macros d'assertions)
#define CATCH_CONFIG_COUNTER

// ------
// Simple toggle defines
// ------

// Tu peux décommenter ces lignes si tu as besoin de fonctionnalités spécifiques
// #define CATCH_CONFIG_FAST_COMPILE
// #define CATCH_CONFIG_DISABLE_EXCEPTIONS

// ------
// "Variable" defines, these have actual values
// ------

// Valeurs par défaut pour le moteur Elevate
#define CATCH_CONFIG_DEFAULT_REPORTER "console"
#define CATCH_CONFIG_CONSOLE_WIDTH 80

#endif // CATCH_USER_CONFIG_HPP_INCLUDED