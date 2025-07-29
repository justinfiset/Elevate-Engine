#pragma once

#include <string>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace Elevate
{
	class Texture;
	using TexturePtr = std::shared_ptr<Texture>;

	enum class TextureSource : uint8_t
	{
		File,        // Loaded from disk
		Generated,   // Created from color or procedural
		RenderTarget // Created as framebuffer texture
	};

	enum class TextureFormat : uint8_t
	{
		EMPTY = 0,
		RGB = 3,
	    RGBA = 4
	};

	enum class TextureType : uint8_t
	{
		Diffuse,
		Specular,
		Normal,
		Height,
		Cubemap
	};

	enum class TextureState : uint8_t
	{
		Empty,
		Unloaded = Empty, // Same as empty but for files
		Loading,
		Ready,
		Loaded = Ready, // Same as ready but for files
		Failed
	};

	struct TextureMetadata
	{
		std::string Name;                           // Logical name -> for display purposes
		std::string Path;                           // The absolute file path
		uint32_t Width = 0;                         // Width in px
		uint32_t Height = 0;                        // Height in px
		uint8_t Channels = 0;                       // 3 for rgb, 4 for rgba
		TextureFormat Format = TextureFormat::RGB;  // ex: EE_RGB, EE_RGBA
		TextureType Usage = TextureType::Diffuse;   // ex: Diffuse, Specular...
		TextureSource Source = TextureSource::File; // Loaded from a file, generated or from a framebuffer
		TextureState State = TextureState::Unloaded;
		// TODO IMPL : bool GenerateMipmaps = true;

		TextureMetadata() = default;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		inline void SetData(unsigned char* data, TextureMetadata meta) {
			m_meta = meta;
			SetDataImpl(data);
		}

		virtual void Bind(uint32_t index = 0) = 0;
		virtual void Unbind() = 0;
		virtual bool IsBound() const = 0;
		virtual void SetDataImpl(unsigned char* data) = 0;
		virtual void* GetNativeHandle() const = 0; // Return a handle to the texture differs from the backend

		inline bool IsTextureLoaded() const { return m_meta.State == TextureState::Loaded; }
		bool MatchesPath(std::string pathToMatch);

		static TexturePtr CreateFromFile(const std::string& path, TextureType usage = TextureType::Diffuse);
		static TexturePtr CreateFromColor(const glm::vec3& color, const std::string& name, uint32_t width = 1, uint32_t height = 1);
		static TexturePtr CreateFromColor(const glm::vec4& color, const std::string& name, uint32_t width = 1, uint32_t height = 1);
		static TexturePtr CreateFromData(unsigned char* data, TextureMetadata& meta);

		// Getters
		// TODO: REMOVE, THE TEXTURE CLASS SHOULD NOT KNOW THE ID -> CONCRETE CLASS CAN , OPENGL != VULKAN FOR IMPL. TEXUTRES
		//inline uint32_t GetID() const { return m_textureID; }

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
	protected:
		TextureMetadata m_meta;
	};
}