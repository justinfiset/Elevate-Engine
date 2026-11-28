// stb pulls in libc headers, so include it before GLDebug.h: that header
// imports Elevate.Foundations and must not precede any textual standard
// includes on GCC's module path.
#include <stb/stb_image.h>

#include <ElevateEngine/Platform/OpenGL/GraphicsAPI.h>
#include <ElevateEngine/Platform/OpenGL/GLDebug.h>
#include <ElevateEngine/Renderer/RendererBackend.h>

import Elevate.Foundations.CoreLogger;

namespace Elevate::RendererBackend
{
	bool CreateCubemapTexture(const char* const paths[6], unsigned int& textureID, const char* skyboxFilePath)
	{
		GLCheck(glGenTextures(1, &textureID));
		GLCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));

		for (int i = 0; i < 6; ++i)
		{
			int width = 0;
			int height = 0;
			int nrChannels = 0;
			unsigned char* data = stbi_load(paths[i], &width, &height, &nrChannels, 0);
			if (!data)
			{
				CoreLogger::Error("Error : Unable to load cubemap texture [{}] : {} with the following skybox file : {}", i, paths[i], skyboxFilePath);
				return false;
			}

			GLCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}

		GLCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

		GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		return true;
	}

	void BindCubemapTexture(unsigned int textureID, unsigned char slot)
	{
		GLCheck(glActiveTexture(GL_TEXTURE0 + slot));
		GLCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
	}
}
