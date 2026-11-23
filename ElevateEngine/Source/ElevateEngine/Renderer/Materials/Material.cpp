module;

#include <memory>
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <cctype>
#include <tuple>
#include <utility>

module Elevate.Renderer.Materials.Material;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Data;

import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;

import Elevate.Renderer.Texture;
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Renderer;

namespace Elevate
{
	MaterialID Material::s_nextId = EE_FIRST_MATERIAL_ID;

	Material::Material()
		: m_id(s_nextId++)
	{
		// Use the default shader asset and resolve it when used
		m_shaderAsset.SetGuid(EE_DEFAULT_SHADER_GUID);
	}

	Material::Material(const std::shared_ptr<Shader>& shader)
		: Material()
	{
		m_shader = shader;
		m_hasExplicitShader = true;

		BuildUniformStorage();
	}

	TypeLayout Material::GetLayout() const
	{
		ResolveShader();

		std::vector<TypeField> allFields;

		TypeField guidField("m_guid", EngineDataType::GUID, GetGuidOffset());
		guidField.data = reinterpret_cast<const char*>(this) + GetGuidOffset();
		allFields.push_back(guidField);

		const size_t shaderAssetOffset = reinterpret_cast<const char*>(&m_shaderAsset) - reinterpret_cast<const char*>(this);
		TypeField shaderAssetField("m_shaderAsset", EngineDataType::ObjectPtr, shaderAssetOffset, "ShaderAsset");
		shaderAssetField.targetTypeKey = TypeRegistry::GetTypeKey<ShaderAsset>();
		shaderAssetField.data = reinterpret_cast<const char*>(this) + shaderAssetOffset;

		allFields.push_back(shaderAssetField);

		if (m_shader)
		{
			const auto& layout = m_shader->GetLayout();

			for (const auto& uniform : layout)
			{
				if (uniform.Type == ShaderDataType::Sampler2D)
				{
					continue;
				}

				if (uniform.Name == ShaderUniforms::ViewProj ||
					uniform.Name == ShaderUniforms::CamPos ||
					uniform.Name == ShaderUniforms::Model)
				{
					continue;
				}

				if (uniform.Index >= m_definedUniforms.size() ||
					!m_definedUniforms[uniform.Index])
				{
					continue;
				}

				TypeField field(uniform.Name, uniform.Type, m_buffer.data() + uniform.Offset);
				allFields.push_back(field);
			}
		}

		return TypeLayout(this, GetName(), allFields);
	}

	void Material::SetTexture(const std::string& name, std::shared_ptr<Texture> texture)
	{
		m_textures[name] = std::move(texture);
	}

	std::shared_ptr<Texture> Material::GetTextureForUniform(const std::string& uniformName) const
	{
		if (m_textures.empty()) return nullptr;

		auto it = m_textures.find(uniformName);
		if (it != m_textures.end()) return it->second;

		static auto containsCaseless = [](std::string_view str, std::string_view sub) {
			return std::search(str.begin(), str.end(), sub.begin(), sub.end(),
				[](char a, char b) { return ::tolower(a) == ::tolower(b); }) != str.end();
			};

		static constexpr std::string_view keywords[] = { "diffuse", "specular", "ambient", "normal" };

		for (const auto& [key, tex] : m_textures)
		{
			for (auto kw : keywords)
			{
				if (containsCaseless(uniformName, kw) && containsCaseless(key, kw))
					return tex;
			}
		}

		return nullptr;
	}

	void Material::Apply()
	{
		ResolveShader();

		if (!m_shader)
		{
			return;
		}

		Renderer::BindShader(m_shader);

		const auto& layout = m_shader->GetLayout();

		for (const auto& uniform : layout)
		{
			if (uniform.Type == ShaderDataType::Sampler2D) continue;

			if (uniform.Name == ShaderUniforms::ViewProj ||
				uniform.Name == ShaderUniforms::CamPos ||
				uniform.Name == ShaderUniforms::Model)
			{
				continue;
			}

			if (m_definedUniforms[uniform.Index])
			{
				m_shader->SetUniform(uniform.Name, uniform.Type, m_buffer.data() + uniform.Offset);
			}
		}

		uint32_t slot = 0;
		std::string hasFlagName;
		hasFlagName.reserve(64);

		for (const auto& uniform : layout)
		{
			if (uniform.Type != ShaderDataType::Sampler2D) continue;

			const std::string& texName = uniform.Name;
			std::shared_ptr<Texture> texture = GetTextureForUniform(texName);

			hasFlagName.assign("has_").append(texName);

			if (texture && texture->IsTextureLoaded() && texture->GetWidth() > 0 && texture->GetHeight() > 0)
			{
				Renderer::BindTexture(texture, slot);
				m_shader->SetUniform1i(texName, slot);
				m_shader->SetUniform1i(hasFlagName, 1);
			}
			else
			{
				Renderer::BindTexture(TextureManager::GetDefaultTexture(), slot);
				m_shader->SetUniform1i(texName, slot);
				m_shader->SetUniform1i(hasFlagName, 0);
			}

			slot++;
		}
	}

	std::shared_ptr<Shader> Material::GetShader() const
	{
		ResolveShader();
		return m_shader;
	}

	MaterialPtr MaterialFactory::Create(const std::shared_ptr<Shader>& shader)
	{
		if (!shader) return nullptr;
		return std::shared_ptr<Material>(new Material(shader));
	}

	MaterialPtr MaterialRegistry::LoadMaterial(const std::shared_ptr<Shader>& shader)
	{
		if (!shader) return nullptr;
		MaterialPtr material = MaterialFactory::Create(shader);
		instance().m_materials[material->GetID()] = material;
		return material;
	}

	MaterialPtr MaterialRegistry::GetMaterial(MaterialID id)
	{
		auto& materials = instance().m_materials;
		auto it = materials.find(id);
		return (it != materials.end()) ? it->second : nullptr;
	}

	void MaterialRegistry::_Init()
	{
		m_materials[EE_DEFAULT_MATERIAL] = MaterialFactory::Create(ShaderManager::GetShader(EE_DEFAULT_SHADER));
		m_materials[EE_ERROR_MATERIAL] = MaterialFactory::Create(ShaderManager::GetShader(EE_ERROR_SHADER));
		CoreLogger::Info("Initialized MaterialRegistry.");
	}

	MaterialRegistry& MaterialRegistry::instance()
	{
		static MaterialRegistry inst;
		return inst;
	}
}