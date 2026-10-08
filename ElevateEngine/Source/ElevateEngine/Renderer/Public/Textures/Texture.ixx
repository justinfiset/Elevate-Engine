module;

#include <string>
#include <memory>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

export module Elevate.Renderer.Textures.Texture;

import Elevate.Renderer.Textures.TextureMetadata;

export namespace Elevate
{
	class Renderer;
	class Texture;

	class Texture
	{
	public:
		virtual ~Texture() = default;

		inline void SetData(unsigned char* data, TextureMetadata& meta) {
			m_meta = meta;
			SetDataImpl(data);
		}

		virtual void* GetNativeHandle() const = 0; // Return a handle to the texture differs from the backend

		inline bool IsTextureLoaded() const { return m_meta.State == TextureState::Loaded; }
		bool MatchesPath(std::string pathToMatch);

		static std::shared_ptr<Texture> CreateFromFile(const std::string& path, TextureType usage = TextureType::Diffuse);
		static std::shared_ptr<Texture> CreateFromFile(const std::string& path, const TextureMetadata& metadata);
		static std::shared_ptr<Texture> CreateFromColor(const glm::vec3& color, const std::string& name, uint32_t width = 1, uint32_t height = 1);
		static std::shared_ptr<Texture> CreateFromColor(const glm::vec4& color, const std::string& name, uint32_t width = 1, uint32_t height = 1);
		static std::shared_ptr<Texture> CreateFromData(const void* data, TextureMetadata& metadata);

		// NOT ALL GETTERS BUT THE MOST USED
		inline const std::string& GetName() const { return m_meta.Name; }
		inline const std::string& GetPath() const { return m_meta.Path; }
		inline const uint32_t GetWidth() const { return m_meta.Width; }
		inline const uint32_t GetHeight() const { return m_meta.Height; }
		inline const TextureType GetUsage() const { return m_meta.Usage; }

		inline const TextureMetadata& GetMetadata() const { return m_meta; }

	protected:
		Texture() = default;
		Texture(TextureMetadata meta) : m_meta(meta) {}

		virtual void SetDataImpl(const void* data) = 0;

	private:
		virtual void Bind(uint32_t index = 0) = 0;
		virtual void Unbind() = 0;

	protected:
		TextureMetadata m_meta;

		friend class Renderer;
	};
}