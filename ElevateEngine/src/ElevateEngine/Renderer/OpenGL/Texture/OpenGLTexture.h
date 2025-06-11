#pragma once

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		//OpenGLTexture(const std::string& path) : OpenGLTexture(path, 0, "textureDiffuse") { }
		OpenGLTexture(const std::string& path, unsigned int index, const std::string type);
		OpenGLTexture(unsigned char* data, int width, int height, int channelCount, const std::string& path);

		virtual void Bind(int index = 0) override;
		virtual void Unbind(int index = 0) override;
		virtual bool IsBound() const override;
		virtual void SetData(unsigned char* data, int width, int height, int channelCount) override;
	private:
		bool m_isBound = false;
	};
}