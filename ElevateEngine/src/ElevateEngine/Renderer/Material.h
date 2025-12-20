#pragma once

#include <memory>
#include <map>
#include <glm/glm.hpp>

#include <ElevateEngine/Renderer/Buffer.h>

namespace Elevate
{
	class Shader;
	class Material;
	using MaterialPtr = std::shared_ptr<Material>;

	typedef uint32_t MaterialID;

	class Material
	{
	public:
		Material();
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

		TexturePtr mainTextures[(int)TextureType::Count];
		std::vector<TexturePtr> additionalTextures;

		MaterialID m_id;
		static MaterialID s_nextId;
	};

	class MaterialRegistry
	{
	public:
		inline MaterialPtr GetMaterial(MaterialID& id)
		{
			if (m_materials.contains(id))
			{
				return m_materials.at(id);
			}
			return nullptr;
		}

	private:
		std::map<MaterialID, MaterialPtr> m_materials;
	};
}