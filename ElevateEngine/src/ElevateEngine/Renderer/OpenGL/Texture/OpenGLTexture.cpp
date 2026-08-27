#include "eepch.h"

#include "OpenGLTexture.h"

#include "ElevateEngine/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <ElevateEngine/Renderer/GraphicsAPI.h>

#include <ElevateEngine/Renderer/GLDebug.h>

namespace Elevate
{
	constexpr GLenum ToInternalFormat(TextureFormat format) {
		switch (format) {
			case TextureFormat::GRAYSCALE:		return GL_R8;       // 8-bit single channel
			case TextureFormat::RGB:			return GL_RGB;		// 8-bit RGB
			case TextureFormat::SRGB:			return GL_SRGB8;
			case TextureFormat::RGBA:			return GL_RGBA8;    // 8-bit RGBA
			case TextureFormat::SRGBA:			return GL_SRGB8_ALPHA8;
			case TextureFormat::RGB16F:			return GL_RGB16F;
			case TextureFormat::RGB32F:			return GL_RGB32F;
			case TextureFormat::RGBA16F:		return GL_RGBA16F;
			case TextureFormat::RGBA32F:		return GL_RGBA32F;
			case TextureFormat::DEPTH:			return GL_DEPTH_COMPONENT24;
			case TextureFormat::EMPTY:
			default:							return GL_RGBA8;
		}
	}

	constexpr GLenum ToOpenGL(TextureFormat format) {
		switch (format) {
			case TextureFormat::GRAYSCALE:			return GL_RED;

			case TextureFormat::RGB:				return GL_RGB;
			case TextureFormat::SRGB:				return GL_RGB;
			case TextureFormat::RGB16F:				return GL_RGB;
			case TextureFormat::RGB32F:				return GL_RGB;

			case TextureFormat::RGBA:				return GL_RGBA;
			case TextureFormat::SRGBA:				return GL_RGBA;
			case TextureFormat::RGBA16F:			return GL_RGBA;
			case TextureFormat::RGBA32F:			return GL_RGBA;

			case TextureFormat::DEPTH:				return GL_DEPTH_COMPONENT;
			case TextureFormat::EMPTY:
			//case TextureFormat::DEPTH16:            return GL_DEPTH_COMPONENT16;
			//case TextureFormat::DEPTH24:            return GL_DEPTH_COMPONENT24;
			//case TextureFormat::DEPTH32F:           return GL_DEPTH_COMPONENT32F;
			//case TextureFormat::DEPTH24_STENCIL8:   return GL_DEPTH24_STENCIL8;
			default:								return GL_RGBA;
		}
	}

	constexpr GLenum ToOpenGLType(TextureFormat format) {
		switch (format) {
		case TextureFormat::RGB16F:				return GL_FLOAT;
		case TextureFormat::RGB32F:				return GL_FLOAT;
		case TextureFormat::RGBA16F:			return GL_FLOAT;
		case TextureFormat::RGBA32F:			return GL_FLOAT;
		case TextureFormat::DEPTH:				return GL_FLOAT;
		default:                                return GL_UNSIGNED_BYTE;
		}
	}

	constexpr GLenum ToOpenGL(TextureFilter filter) {
		switch (filter) {
			case TextureFilter::Nearest: return GL_NEAREST;
			case TextureFilter::Linear:  return GL_LINEAR;
			default:                     return GL_NEAREST;
		}
	}

	constexpr GLenum ToOpenGL(TextureWrap wrap) {
		switch (wrap) {
			case TextureWrap::Repeat:       return GL_REPEAT;
			case TextureWrap::MirrorRepeat: return GL_MIRRORED_REPEAT;
			case TextureWrap::ClampToEdge:  return GL_CLAMP_TO_EDGE;
			case TextureWrap::ClampToBorder:return GL_CLAMP_TO_BORDER;
			default:                        return GL_REPEAT;
		}
	}

	constexpr GLenum ToOpenGL(TextureType type) {
		switch (type) {
		case TextureType::Cubemap: return GL_TEXTURE_CUBE_MAP;
		default:                   return GL_TEXTURE_2D;
		}
	}

	constexpr GLenum GetMinFilter(TextureFilter filter, bool useMipmaps) {
		if (!useMipmaps) {
			return (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;
		}
		return (filter == TextureFilter::Nearest) ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;
	}

	OpenGLTexture::OpenGLTexture(const TextureMetadata& meta)
		: Texture(meta)
	{
		GLCheck(glGenTextures(1, &m_textureID));
	}

	OpenGLTexture::OpenGLTexture(const void* data, const TextureMetadata& meta)
		: OpenGLTexture(meta)
	{
		Bind();
		SetDataImpl(data);
	}

	void OpenGLTexture::Bind(uint32_t index)
	{
		GLCheck(glActiveTexture(GL_TEXTURE0 + index));
		GLCheck(glBindTexture(ToOpenGL(m_meta.Usage), m_textureID));
	}

	void OpenGLTexture::Unbind()
	{
		GLCheck(glBindTexture(ToOpenGL(m_meta.Usage), 0));
	}

	void OpenGLTexture::SetDataImpl(const void* data)
	{
		Bind();

		GLCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGL(m_meta.WrapS)));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGL(m_meta.WrapT)));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetMinFilter(m_meta.MinFilter, m_meta.Mipmaps)));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToOpenGL(m_meta.MagFilter)));

		if (m_meta.Usage == TextureType::ShadowMap)
		{
			GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
			GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));

#if defined(EE_PLATFORM_WEB)
			GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
#else		// Not supported on WebGL
			constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			GLCheck(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
#endif
		}

#ifdef EE_SUPPORTS_DSA
		if (m_meta.Format == TextureFormat::GRAYSCALE) {
			GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
			GLCheck(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
		}
#endif

		EE_CORE_INFO("Creating Texture: ID={}, Size={}x{}, Format={}", m_textureID, m_meta.Width, m_meta.Height, (int)m_meta.Format);

		uint32_t width = m_meta.Width > 0 ? m_meta.Width : 1;
		uint32_t height = m_meta.Height > 0 ? m_meta.Height : 1;

		const void* pixelsToUpload = data;

		GLCheck(glTexImage2D(
			GL_TEXTURE_2D,
			0,
			ToInternalFormat(m_meta.Format),
			width,
			height,
			0,
			ToOpenGL(m_meta.Format),
			ToOpenGLType(m_meta.Format),
			pixelsToUpload
		));

		if (m_meta.Mipmaps && width > 1 && height > 1 && pixelsToUpload != nullptr) {
			GLCheck(glGenerateMipmap(GL_TEXTURE_2D));
		}
	}

	void* OpenGLTexture::GetNativeHandle() const
	{
		return reinterpret_cast<void*>((intptr_t)m_textureID);
	}
}