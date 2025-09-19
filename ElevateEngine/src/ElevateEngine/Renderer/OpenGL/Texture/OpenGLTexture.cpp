#include "eepch.h"

#include "OpenGLTexture.h"

#include "ElevateEngine/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <glad/glad.h>

#include <ElevateEngine/Renderer/GLDebug.h>

namespace Elevate
{
	constexpr GLenum ToInternalFormat(TextureFormat format) {
		switch (format) {
		case TextureFormat::GRAYSCALE:   return GL_R8;       // 8-bit single channel
		case TextureFormat::RGB:         return GL_RGB8;     // 8-bit RGB
		case TextureFormat::RGBA:        return GL_RGBA8;    // 8-bit RGBA
		case TextureFormat::DEPTH:       return GL_DEPTH_COMPONENT24;
		default:                         return GL_RGBA8;
		}
	}

	constexpr GLenum ToOpenGL(TextureFormat format) {
		switch (format) {
		case TextureFormat::EMPTY:              return GL_NONE;
		case TextureFormat::GRAYSCALE:			return GL_RED;
		case TextureFormat::RGB:                return GL_RGB;
		case TextureFormat::RGBA:               return GL_RGBA;
		case TextureFormat::DEPTH:				return GL_DEPTH_COMPONENT;
			//case TextureFormat::DEPTH16:            return GL_DEPTH_COMPONENT16;
			//case TextureFormat::DEPTH24:            return GL_DEPTH_COMPONENT24;
			//case TextureFormat::DEPTH32F:           return GL_DEPTH_COMPONENT32F;
			//case TextureFormat::DEPTH24_STENCIL8:   return GL_DEPTH24_STENCIL8;
		default:                                return GL_NONE;
		}
	}

	constexpr GLenum ToOpenGLType(TextureFormat format) {
		switch (format) {
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

	OpenGLTexture::OpenGLTexture(unsigned char* data, TextureMetadata& meta)
		: Texture(meta)
	{
		// todo get parameters for the textures
		GLCheck(glGenTextures(1, &m_textureID));

		Bind();
		SetDataImpl(data);
	}

	void OpenGLTexture::Bind(uint32_t index)
	{
		GLCheck(glActiveTexture(GL_TEXTURE0 + index));
		GLCheck(glBindTexture(ToOpenGL(m_meta.Usage), m_textureID));
		m_isBound = true;
	}

	void OpenGLTexture::Unbind()
	{
		GLCheck(glBindTexture(ToOpenGL(m_meta.Usage), 0));
		m_isBound = false;
	}

	bool OpenGLTexture::IsBound() const
	{
		return m_isBound;
	}

	void OpenGLTexture::SetDataImpl(unsigned char* data)
	{
		Bind();
		
		// set the texture wrapping parameters	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGL(m_meta.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGL(m_meta.WrapT));
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToOpenGL(m_meta.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToOpenGL(m_meta.MagFilter));

		// Swizzle if there is only a single channnel
		if (m_meta.Format == TextureFormat::GRAYSCALE) {
			GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}

		if (data || m_meta.Source == TextureSource::RenderTarget) 
		{
			GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, ToInternalFormat(m_meta.Format), m_meta.Width, m_meta.Height, 0, ToOpenGL(m_meta.Format), ToOpenGLType(m_meta.Format), data));

			if (m_meta.Mipmaps)
				GLCheck(glGenerateMipmap(GL_TEXTURE_2D));
		}
	}

	void* OpenGLTexture::GetNativeHandle() const
	{
		return reinterpret_cast<void*>((intptr_t)m_textureID);
	}
}