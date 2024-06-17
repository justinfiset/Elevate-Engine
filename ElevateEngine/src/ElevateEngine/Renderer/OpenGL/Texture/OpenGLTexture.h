#pragma once

#include "ElevateEngine/Renderer/Texture.h"

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(std::string path);
		virtual void Bind(int index = 0) const override;

	private:
		unsigned int m_TextureID;
	};
}