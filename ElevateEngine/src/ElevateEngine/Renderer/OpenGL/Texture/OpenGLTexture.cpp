#include "eepch.h"

#include "OpenGLTexture.h"

#include "ElevateEngine/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <glad/glad.h>
namespace Elevate
{
	OpenGLTexture::OpenGLTexture(const std::string& path, unsigned int index, const std::string type) : Texture(path, type)
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		// set the texture wrapping parameters	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;

		// FLIPPING THE IMAGE VERTICALLY - NOT NEEDED UNLESS WE FLIP UVS ON THE MODELS BUT USEFUL FOR IMAGES ETC
		//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data)
		{
			//EE_CORE_TRACE("{0}, {1}, {2}", width, height, nrChannels);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			EE_CORE_TRACE("Unable to load texture : {0}", path.c_str());
		}
	}

	OpenGLTexture::OpenGLTexture(unsigned char* data, int width, int height, int channelCount, const std::string& path)
		: Texture(path)
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
			SetData(data, width, height, channelCount);
		}
	}

	void OpenGLTexture::Bind(int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);

		if (m_type == EE_TEXTURE_CUBEMAP)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		}
		else 
		{
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}

		m_isBound = true;
	}

	void OpenGLTexture::Unbind(int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);

		if (m_type == EE_TEXTURE_CUBEMAP)
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

	void OpenGLTexture::SetData(unsigned char* data, int width, int height, int channelCount)
	{
		Bind();

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			EE_CORE_TRACE("Unable to load texture : {0}", m_path.c_str());
		}
	}
}
