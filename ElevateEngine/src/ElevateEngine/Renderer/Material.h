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

		template<typename T>
		void Set(const std::string& name, const T& value)
		{
			for (const auto& element : m_layout)
			{
				if (element.Name == name)
				{
					memcpy(m_buffer.data() + element.Offset, &value, sizeof(T));
				}
			}
		}

		void Apply();
		std::shared_ptr<Shader> GetShader();
	private:
		std::shared_ptr<Shader> m_shader;
		// Uniforms
		BufferLayout m_layout;
		std::vector<uint8_t> m_buffer;
	};
}