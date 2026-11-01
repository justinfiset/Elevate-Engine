module;

#include "Reflection.h"

export module Elevate.Renderer.Camera:CameraProjectionMode;

import Elevate.Foundations.Enums;

export namespace Elevate
{
	enum class CameraProjectionMode : EnumType
	{
		Perspective = 0,
		Orthographic = 1
	};
}

// Register the enum with the reflection system
namespace Elevate
{
	BEGIN_ENUM(CameraProjectionMode)
		ENUM_VALUE(Perspective)
		ENUM_VALUE(Orthographic)
		END_ENUM(CameraProjectionMode)
}
