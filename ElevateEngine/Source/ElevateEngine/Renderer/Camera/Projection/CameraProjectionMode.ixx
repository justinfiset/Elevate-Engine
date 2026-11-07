module;

#include "Reflection.h"

export module Elevate.Renderer.Camera:CameraProjectionMode;

import Elevate.Foundations.Enums;
import Elevate.Core.Types.TypeRegistry;

export namespace Elevate
{
	enum class CameraProjectionMode : EnumType
	{
		Perspective = 0,
		Orthographic = 1
	};

	BEGIN_ENUM(CameraProjectionMode)
		ENUM_VALUE(Perspective)
		ENUM_VALUE(Orthographic)
		END_ENUM(CameraProjectionMode);
}
