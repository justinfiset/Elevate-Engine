#include "eepch.h"
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

#include <ElevateEngine/Renderer/OpenGL/Texture/OpenGLTexture.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>

namespace Elevate
{
	TexturePtr Texture::Create(std::string path)
	{
		TexturePtr texture = TextureManager::GetTexture(path);
		if (texture) return texture;

		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: texture = nullptr; break; // TODO implement
		case RendererAPI::GraphicAPI::OpenGL: texture = std::make_shared<OpenGLTexture>(path);
		}

		return TextureManager::LoadTexture(texture);

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
	}

	TexturePtr Texture::Create(std::string path, unsigned int index, std::string type)
	{
		// TODO IMPL WITH TEXTURE MANAGER

		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break; // TODO implement
		case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLTexture>(path, index, type);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
	}
}
