module;

#include <string>
#include <cstdint>

export module Elevate.Renderer.Textures.TextureMetadata;

export namespace Elevate
{
	enum class TextureSource : uint8_t
	{
		File,		 // Loaded from disk
		Generated,	 // Created from color or procedural
		RenderTarget // Created as framebuffer texture
	};

	enum class TextureFormat : uint8_t
	{
		EMPTY = 0,
		GRAYSCALE,

		RGB,
		RGBA,
		RGB16F,
		RGB32F,
		RGBA16F,
		RGBA32F,

		SRGB,
		SRGBA,

		DEPTH,
		DEPTH16,
		DEPTH24,
		DEPTH32F,

		DEPTH24_STENCIL8 // To use depth and stencil at the same time
	};

	enum class TextureType : uint8_t
	{
		Diffuse,
		Specular,
		Normal,
		Height,
		Cubemap,
		Ambient,
		AmbientOcclusion,
		Depth,
		ShadowMap,
		Count
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

	enum class TextureFilter : uint8_t
	{
		Nearest,
		Linear
	};

	enum class TextureWrap : uint8_t
	{
		Repeat,
		MirrorRepeat,
		ClampToEdge,
		ClampToBorder
	};
}

namespace Elevate
{
	uint8_t GetTextureFormatChannels(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::GRAYSCALE:
			return 1;
		case TextureFormat::RGB:
			return 3;
		case TextureFormat::RGB16F:
			return 3;
		case TextureFormat::RGB32F:
			return 3;
		case TextureFormat::RGBA:
			return 4;
		case TextureFormat::RGBA16F:
			return 4;
		case TextureFormat::RGBA32F:
			return 4;
		case TextureFormat::SRGB:
			return 3;
		case TextureFormat::SRGBA:
			return 4;
		case TextureFormat::DEPTH:
			return 1;
		case TextureFormat::DEPTH16:
			return 1;
		case TextureFormat::DEPTH24:
			return 1;
		case TextureFormat::DEPTH32F:
			return 1;
		case TextureFormat::DEPTH24_STENCIL8:
			return 2;
		case TextureFormat::EMPTY:
		default:
			return 0;
		}
	}
}

export namespace Elevate
{
	struct TextureMetadata
	{
		std::string Name;							 // Logical name -> for display purposes
		std::string Path;							 // The absolute file path
		uint32_t Width = 0;							 // Width in px
		uint32_t Height = 0;						 // Height in px
		uint8_t Channels = 0;						 // 3 for rgb, 4 for rgba
		TextureFormat Format = TextureFormat::RGB;	 // ex: EE_RGB, EE_RGBA
		TextureType Usage = TextureType::Diffuse;	 // ex: Diffuse, Specular...
		TextureSource Source = TextureSource::File;	 // Loaded from a file, generated or from a framebuffer
		TextureState State = TextureState::Unloaded; // General state of the texxture, unloaded, loaded, failed etc.
		uint32_t Layer = 0;							 // Default: 0 -> Used for cubemap faces, texture arrays, etc.

		TextureFilter MinFilter = TextureFilter::Linear;
		TextureFilter MagFilter = TextureFilter::Linear;
		TextureWrap WrapS = TextureWrap::Repeat;
		TextureWrap WrapT = TextureWrap::Repeat;
		bool Mipmaps = true;

		TextureMetadata() = default;
	};

	struct TextureMetadataBuilder
	{
		TextureMetadata data;

		TextureMetadataBuilder() = default;
		TextureMetadataBuilder(TextureMetadata& base) : data(base) {}

		TextureMetadataBuilder& Name(const std::string name)
		{
			data.Name = name;
			return *this;
		}
		TextureMetadataBuilder& Path(const std::string& path)
		{
			data.Path = path;
			return *this;
		}
		TextureMetadataBuilder& size(const uint32_t w, const uint32_t h)
		{
			data.Width = w;
			data.Height = h;
			return *this;
		}
		TextureMetadataBuilder& Format(const TextureFormat fmt)
		{
			data.Format = fmt;
			data.Channels = GetTextureFormatChannels(fmt);
			return *this;
		}
		TextureMetadataBuilder& Usage(const TextureType type)
		{
			data.Usage = type;
			return *this;
		}
		TextureMetadataBuilder& Source(const TextureSource src)
		{
			data.Source = src;
			return *this;
		}
		TextureMetadataBuilder& State(const TextureState state)
		{
			data.State = state;
			return *this;
		}
		TextureMetadataBuilder& Layer(const uint32_t layer)
		{
			data.Layer = layer;
			return *this;
		}
		TextureMetadataBuilder& Filter(const TextureFilter min, const TextureFilter mag)
		{
			data.MinFilter = min;
			data.MagFilter = mag;
			return *this;
		}
		TextureMetadataBuilder& Wrap(const TextureWrap s, const TextureWrap t)
		{
			data.WrapS = s;
			data.WrapT = t;
			return *this;
		}
		TextureMetadataBuilder& Mipmaps(const bool mipmaps)
		{
			data.Mipmaps = mipmaps;
			return *this;
		}
		TextureMetadata Build() { return data; }
	};
}
