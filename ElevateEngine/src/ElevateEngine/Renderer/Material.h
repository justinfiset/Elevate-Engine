#pragma once

#include <memory>
#include <map>
#include <glm/glm.hpp>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Renderer/Buffer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

#define EE_DEFAULT_MATERIAL 0

namespace Elevate
{
	class Material;
	class MaterialFactory;
	class MaterialRegistry;
	using MaterialPtr = std::shared_ptr<Material>;

	typedef uint32_t MaterialID;

	class Material
	{
	public:
		template<typename T>
		void Set(const std::string& name, const T& value)
		{
			for (const auto& element : m_shader->GetLayout())
			{
				if (element.Name == name)
				{
					memcpy(m_buffer.data() + element.offset, &value, sizeof(T));
				}
			}
		}

		void Apply();
		std::shared_ptr<Shader> GetShader();
		inline MaterialID GetID() { return m_id; }

	private:
		Material();
		Material(const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> m_shader;
		// Uniforms
		std::vector<uint8_t> m_buffer;

		TexturePtr m_mainTextures[(int)TextureType::Count];
		std::vector<TexturePtr> m_additionalTextures;

		MaterialID m_id;
		static MaterialID s_nextId;

		friend class MaterialFactory;
	};

	class MaterialFactory
	{
	protected:
		static MaterialPtr Create(const std::shared_ptr<Shader>& shader);
		friend class MaterialRegistry;
	};

	class MaterialRegistry
	{
	public:
		static MaterialPtr LoadMaterial(const std::shared_ptr<Shader>& shader);
		static MaterialPtr GetMaterial(MaterialID id);

	private:
		MaterialRegistry();
		static MaterialRegistry& instance();

		std::unordered_map<MaterialID, MaterialPtr> m_materials;
	};
}