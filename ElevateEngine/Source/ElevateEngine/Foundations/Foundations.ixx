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
export import Elevate.Foundations.Bytes;
export import Elevate.Foundations.Guid;

// Path + Files
export import Elevate.Foundations.Paths;
