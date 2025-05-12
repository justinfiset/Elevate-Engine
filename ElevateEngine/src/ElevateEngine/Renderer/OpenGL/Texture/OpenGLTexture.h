#pragma once

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		// TODO remove raw string in script, find a better implemtnation;
		OpenGLTexture(std::string path) : OpenGLTexture(path, 0, "textureDiffuse") { }
		OpenGLTexture(std::string path, unsigned int index, std::string type);
		OpenGLTexture(char* data, int width, int height, int channelCount, const std::string& path);

		virtual void Bind(int index = 0) override;
		virtual void Unbind(int index = 0) override;
		virtual bool IsBound() const override;

	private:
		// TODO IMPL. BETTER SOLUTION TO PREVENT USER ERRORS
		bool m_IsBound = false; // ON NE PEUT UTILISER CETTE VARIABLE QUE SI ON UNBIND ABOSLUMENT
	};
}