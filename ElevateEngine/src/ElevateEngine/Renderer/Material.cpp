#include "Material.h"

#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Buffer.h>

namespace Elevate
{
	Material::Material(const std::shared_ptr<Shader>& shader, const BufferLayout& layout)
	{
		m_shader = shader;
		m_layout = layout;
	}

	void Material::Apply()
	{
		Renderer::BindShader(m_shader);
		m_shader->SetModelMatrix(modelMatrix);
		// todo : find a way to apply uniforms based on the shader

		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Bind(i);
		}
	}

	std::shared_ptr<Shader> Material::GetShader()
	{
		return m_shader;
	}
}