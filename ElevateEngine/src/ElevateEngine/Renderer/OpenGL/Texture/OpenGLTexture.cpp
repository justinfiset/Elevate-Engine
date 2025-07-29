#include "eepch.h"

#include "OpenGLTexture.h"

#include "ElevateEngine/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <glad/glad.h>
namespace Elevate
{
	OpenGLTexture::OpenGLTexture(unsigned char* data, TextureMetadata& meta) 
		: Texture(meta)
	{
		// todo get parameters for the textures
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		// set the texture wrapping parameters	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (data)
		{
			SetDataImpl(data);
		}
	}

	void OpenGLTexture::Bind(uint32_t index)
	{
		glActiveTexture(GL_TEXTURE0 + index);

		if (m_meta.Usage == TextureType::Cubemap)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		}
		else 
		{
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}

		m_isBound = true;
	}

	void OpenGLTexture::Unbind()
	{
		if(m_meta.Usage == TextureType::Cubemap)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		m_isBound = false;
	}

	bool OpenGLTexture::IsBound() const
	{
		return m_isBound;
	}

	void OpenGLTexture::SetDataImpl(unsigned char* data)
	{
		Bind();

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_meta.Width, m_meta.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			EE_CORE_TRACE("Unable to load texture : {0}, {1}", m_meta.Name.c_str(), m_meta.Path.c_str());
		}
	}

	void* OpenGLTexture::GetNativeHandle() const
	{
		return reinterpret_cast<void*>((intptr_t)m_textureID);
	}
}
