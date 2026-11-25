module;

#include <glm/vec4.hpp>

export module Elevate.Editor.Colors;

export namespace Elevate::Editor::Colors
{
	inline glm::vec4 Error() { return glm::vec4(166.0f, 25.0f, 46.0f, 255.0f); }
	inline glm::vec4 Warning() { return glm::vec4(255.0f, 153.0f, 0.0f, 255.0f); }
	inline glm::vec4 Message() { return glm::vec4(255.0f, 255.0f, 255.0f, 255.0f); }
	inline glm::vec4 Success() { return glm::vec4(4.0f, 179.0f, 9.0f, 255.0f); }
	inline glm::vec4 Information() { return glm::vec4(0.0f, 87.0f, 184.0f, 255.0f); }
}
