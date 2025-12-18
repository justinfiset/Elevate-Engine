#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Elevate
{
	class Shader;
	class BufferLayout;

	class Material;
	using MaterialPtr = std::shared_ptr<Material>;

	class Material
	{
	public:
		Material(const std::shared_ptr<Shader>& shader, const BufferLayout& layout);

		void Apply();
		std::shared_ptr<Shader> GetShader();
	private:
		std::shared_ptr<Shader> m_shader;
		BufferLayout m_layout;
	};
}