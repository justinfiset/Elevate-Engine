module;

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Reflection.h"

export module Elevate.Renderer.Materials.Material;

// Foundations
import Elevate.Foundations.Assert;
// Core
import Elevate.Core.Assets.Asset;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Types.TypeLayout;
// Renderer
import Elevate.Renderer.Texture;
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Buffers;
import Elevate.Renderer.Commands.RenderBucketType;
import Elevate.Renderer.RenderState;

export namespace Elevate
{
	class Material;
	class MaterialFactory;
	class MaterialRegistry;

	using MaterialPtr = EEObjectPtr<Material>;
	using MaterialID = uint32_t;

	inline constexpr MaterialID EE_DEFAULT_MATERIAL = 0;
	inline constexpr MaterialID EE_ERROR_MATERIAL = 1;
	inline constexpr MaterialID EE_FIRST_MATERIAL_ID = 2;

	class Material : public Asset
	{
		BEGIN_OBJECT(Material, EE_Asset("Material", ".mat", EE_CreateAssetMenu, EE_AssetColor(0.8f, 0.2f, 0.2f)));

	public:
		using ShaderBinder = bool (*)(const std::shared_ptr<Shader>&);
		using TextureBinder = void (*)(const std::shared_ptr<Texture>&, uint8_t);

		static void SetRendererBindings(ShaderBinder shaderBinder, TextureBinder textureBinder);

		TypeLayout GetLayout() const override;

		template<typename T>
		void Set(const std::string& name, const T& value)
		{
			ResolveShader();

			for (const auto& uniform : m_shader->GetLayout())
			{
				if (uniform.Name == name)
				{
					Assert::That(uniform.Offset + sizeof(T) <= m_buffer.size(),
						"Set Uniform: data size greater then buffer size!");

					std::memcpy(m_buffer.data() + uniform.Offset, &value, sizeof(T));
					m_definedUniforms[uniform.Index] = true;
					return;
				}
			}

			CoreLogger::Warn("Uniform '{0}' not found in the shader : {1}", name, m_shader->GetID());
		}

		void SetTexture(const std::string& name, std::shared_ptr<Texture> texture);
		void Apply();

		std::shared_ptr<Shader> GetShader() const;
		MaterialID GetID() const { return m_id; }

		RenderState& GetRenderState() { return m_state; }
		const RenderState& GetRenderState() const { return m_state; }
		void SetRenderState(const RenderState& state) { m_state = state; }

		RenderBucketType GetBucket() const { return m_bucket; }
		void SetBucket(RenderBucketType bucket) { m_bucket = bucket; }

		size_t GetTextureCount() const { return m_textures.size(); }

		Material();

	private:
		static ShaderBinder s_shaderBinder;
		static TextureBinder s_textureBinder;

		Material(const std::shared_ptr<Shader>& shader);

		void BuildUniformStorage() const
		{
			if (!m_shader)
			{
				return;
			}

			m_buffer.assign(m_shader->GetLayout().GetStride(), 0);
			m_definedUniforms.assign(m_shader->GetLayout().GetElements().size(), false);
		}

		void ResolveShader() const
		{
			if (m_hasExplicitShader)
			{
				return;
			}

			if (m_shaderAsset)
			{
				m_shader = m_shaderAsset->GetShader();
				BuildUniformStorage();
			}
		}

		std::shared_ptr<Texture> GetTextureForUniform(const std::string& uniformName) const;

		bool m_hasExplicitShader = false;
		mutable EEObjectPtr<Shader> m_shader{ nullptr };

		EEObjectPtr<ShaderAsset> m_shaderAsset{ nullptr };
		PROPERTY(m_shaderAsset);

		mutable std::vector<uint8_t> m_buffer;
		mutable std::vector<bool> m_definedUniforms;

		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

		MaterialID m_id{ 0 };
		static MaterialID s_nextId;

		RenderState m_state;
		RenderBucketType m_bucket = RenderBucketType::GBuffer;

		friend class MaterialFactory;

		END_OBJECT_CUSTOM();
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

		static void Init()
		{
			instance()._Init();
		}

	private:
		MaterialRegistry() = default;

		static MaterialRegistry& instance();
		void _Init();

		std::unordered_map<MaterialID, MaterialPtr> m_materials;
	};
}