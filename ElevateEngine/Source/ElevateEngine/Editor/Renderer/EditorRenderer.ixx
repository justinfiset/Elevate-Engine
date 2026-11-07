module;

#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

export module Elevate.Editor.Renderer;

import Elevate.Renderer.Texture;

export namespace Elevate
{
	class EditorRenderer
	{
	public:
		static void DrawBillboard(const glm::vec3 &position, std::shared_ptr<Texture> icon, float scale = 0.5f, glm::vec4 colorModifier = glm::vec4(1.0f));
	};
}
