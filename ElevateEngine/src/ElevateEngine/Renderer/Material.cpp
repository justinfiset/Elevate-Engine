#include "Material.h"

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Buffer.h>

namespace Elevate
{
	MaterialID Material::s_nextId = EE_DEFAULT_MATERIAL + 1; // Keep 0 for a default material

	Material::Material()
	{
		m_id = s_nextId++;
	}

	Material::Material(const std::shared_ptr<Shader>& shader)
		: Material()
	{
		m_shader = shader;

		if (shader)
		{
			m_buffer.resize(m_shader->GetLayout().GetStride());
		}
		else
		{
			EE_CORE_ERROR("A Material can only be created from a valid Shader instance.");
		}
	}

	void Material::Apply()
	{
		if (m_shader)
		{
			Renderer::BindShader(m_shader);

			for (const auto& uniform : m_shader->GetLayout())
			{
				// ignore system managed uniforms
				if (uniform.Name == EE_SHADER_VIEWPROJ ||
					uniform.Name == EE_SHADER_CAMPOS ||
					uniform.Name == EE_SHADER_MODEL)
				{
					continue;
				}

				void* data = m_buffer.data() + uniform.offset;
				m_shader->SetUniform(uniform.Name, uniform.Type, data);
			}
		}
		else
		{
			EE_CORE_ERROR("Material::Apply() : Cannot apply uniforms to a shader that is nullptr.");
		}


		// todo : find a way to textures uniforms based on the shader

		//for (unsigned int i = 0; i < m_Textures.size(); i++)
		//{
		//	m_Textures[i]->Bind(i);	
		//}
	}

	std::shared_ptr<Shader> Material::GetShader()
	{
		return m_shader;
	}

	MaterialPtr MaterialFactory::Create(const std::shared_ptr<Shader>& shader)
	{
		if (!shader)
		{
			EE_CORE_ERROR("(MaterialFactory) : Cannot create a material from a null shader.");
			return nullptr;
		}
		return std::shared_ptr<Material>(new Material(shader));
	}

	MaterialPtr MaterialRegistry::LoadMaterial(const std::shared_ptr<Shader>& shader)
	{
		EE_TRACE("(MaterialRegistry) : Creating shader for shader : {}", shader->GetRendererID());
		MaterialPtr material = MaterialFactory::Create(shader);
		instance().m_materials[material->GetID()] = material;
		return material;
	}

	MaterialPtr MaterialRegistry::GetMaterial(MaterialID id)
	{
		if (instance().m_materials.contains(id))
		{
			return instance().m_materials.at(id);
		}
		return nullptr;
	}

	MaterialRegistry::MaterialRegistry()
	{
		EE_CORE_INFO("Initializing MaterialRegistry.");
		// Create a default material for the default shader
		EE_CORE_TRACE("(MaterialRegistry) : Creating default material from default shader.");
		m_materials[EE_DEFAULT_MATERIAL] = MaterialFactory::Create(ShaderManager::GetShader(EE_DEFAULT_SHADER));
		EE_CORE_INFO("Initialized MaterialRegistry.");
	}

	MaterialRegistry& MaterialRegistry::instance()
	{
		static MaterialRegistry instance;
		return instance;
	}
}