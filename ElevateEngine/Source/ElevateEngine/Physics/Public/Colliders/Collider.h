#pragma once

// Scene is imported so reflection macros in derived collider headers see a
// complete GameObject. Reflection.h is deliberately not included here: every
// textual include must stay before this header's first import.
import Elevate.Core.Scene;
import Elevate.Physics.Collider;
