module;

#include "Core.h"
#include "Assert.h"

export module Elevate.Foundations;

// Logging + Assertion
export import Elevate.Foundations.Logger;
#ifdef EE_ENGINE_BUILD
export import Elevate.Foundations.CoreLogger;
export import Elevate.Foundations.Assert;
#endif

// Low level types
export import Elevate.Foundations.Data;
export import Elevate.Foundations.Bytes;
export import Elevate.Foundations.Guid;
export import Elevate.Foundations.Enums;

// Path + Files
export import Elevate.Foundations.Paths;
export import Elevate.Foundations.Files;

// Low Level Functions
export import Elevate.Foundations.Shell;
