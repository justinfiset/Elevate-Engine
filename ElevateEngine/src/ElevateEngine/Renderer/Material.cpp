#include "Material.h"

#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Buffer.h>

namespace Elevate
{
	MaterialID Material::s_nextId = 1; // Keep 0 for a default material

	Material::Material()
	{
		m_id = s_nextId++;
	}

	Material::Material(const std::shared_ptr<Shader>& shader, const BufferLayout& layout)
		: m_shader(shader), m_layout(layout)
	{
		m_buffer.resize(m_layout.GetStride());
	}

	void Material::Apply()
	{
		Renderer::BindShader(m_shader);

		for (const auto& uniform : m_layout)
		{
			void* data = m_buffer.data() + uniform.Offset;
			m_shader->SetUniform(uniform.Name, uniform.Type, data);
		}

		// todo : find a way to apply uniforms based on the shader

		//for (unsigned int i = 0; i < m_Textures.size(); i++)
		//{
		//	m_Textures[i]->Bind(i);
		//}
	}

	std::shared_ptr<Shader> Material::GetShader()
	{
		return m_shader;
	}
}