#include "eepch.h"
#include "OpenGLCubemapTexture.h"
#include <glad/glad.h>
#include <stb/stb_image.h>

Elevate::OpenGLCubemapTexture::OpenGLCubemapTexture(std::string path)
{

}

void Elevate::OpenGLCubemapTexture::Bind(int index) const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}

void Elevate::OpenGLCubemapTexture::Unbind(int index) const
{
}
