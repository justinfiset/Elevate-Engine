#include "eepch.h"
#include "Texture.h"
#include "Renderer.h"

#include "OpenGL/Texture/OpenGLTexture.h"

namespace Elevate
{
	Texture* Texture::Create(std::string path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break; // TODO implement
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLTexture(path);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!")
	}

	Texture* Texture::Create(std::string path, unsigned int index, std::string type)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break; // TODO implement
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLTexture(path, index, type);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!")
	}
}
