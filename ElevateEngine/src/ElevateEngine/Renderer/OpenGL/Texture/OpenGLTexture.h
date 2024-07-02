#pragma once

#include "ElevateEngine/Renderer/Texture.h"

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		// TODO remove raw string in script, find a better implemtnation;
		OpenGLTexture(std::string path) : OpenGLTexture(path, 0, "textureDiffuse") { }
		OpenGLTexture(std::string path, unsigned int index, std::string type);
		virtual void Bind(int index = 0) const override;
		virtual void Unbind(int index = 0) const override;
	};
}