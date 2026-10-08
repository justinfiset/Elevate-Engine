module;

#include <ElevateEngine/Core/Reflection/Reflection.h>

export module Elevate.Core.Scene.SceneType;

export import Elevate.Foundations.Enums;

export namespace Elevate
{
	enum class SceneType : EnumType
	{
		RuntimeScene = 0,
		EditorScene = 1,
		DebugScene = 99
	};
}

namespace Elevate
{
	BEGIN_ENUM(SceneType)
		ENUM_VALUE(RuntimeScene)
		ENUM_VALUE(EditorScene)
		ENUM_VALUE(DebugScene)
		END_ENUM(SceneType)
}
