module;

#include <glm/glm.hpp>

export module Elevate.Renderer.Debug.Types;

#if defined(EE_EDITOR_BUILD) || defined(EE_DEBUG)

export namespace Elevate
{
	struct DebugPoint
	{
		glm::vec3 pos;
	};

	struct DebugLine
	{
		glm::vec3 start;
		glm::vec3 end;
		glm::vec4 color;
		float width;
		float lifetime; // duration in seconds, 0 for a single frame, < 0 if infinite
	};
}

#endif // EE_EDITOR_BUILD || EE_DEBUG
