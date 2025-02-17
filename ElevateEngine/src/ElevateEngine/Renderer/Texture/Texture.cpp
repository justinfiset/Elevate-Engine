#include "eepch.h"
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

#include <ElevateEngine/Renderer/OpenGL/Texture/OpenGLTexture.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>

#include <filesystem>

namespace Elevate
{
	TexturePtr Texture::Create(std::string path)
	{
		// TODO do for all other constructors / factory methods
		std::filesystem::path abs = std::filesystem::absolute(path);
		//EE_CORE_TRACE(path);
		TexturePtr texture = TextureManager::GetTexture(abs.string());
		if (texture) return texture;

		//EE_CORE_TRACE(abs.string()); // Uncomment to see all loaded texture wich are actually created each frame

		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: texture = nullptr; break; // TODO implement
		case RendererAPI::GraphicAPI::OpenGL: texture = std::make_shared<OpenGLTexture>(abs.string());
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

	bool Texture::MatchesPath(std::string pathToMatch)
	{
		std::filesystem::path otherPath = pathToMatch;
		if (otherPath.is_absolute()) {
			return pathToMatch == this->m_Path;
		}
		else {
			return std::filesystem::absolute(otherPath).string() == this->m_Path;
		}
	}
}
