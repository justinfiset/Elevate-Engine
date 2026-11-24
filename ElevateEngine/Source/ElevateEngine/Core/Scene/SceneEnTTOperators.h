#pragma once

// GCC and Clang attach declarations from headers included in a module's
// purview to that module's BMI.  EnTT's sparse_set and dense_map iterator
// operators must be visible in each translation unit's global module fragment,
// otherwise inline Scene code cannot find them by ADL when it imports the
// Scene BMI.  Include this header in the global module fragment of any unit
// that instantiates that code.
#include <entt/entity/registry.hpp>
