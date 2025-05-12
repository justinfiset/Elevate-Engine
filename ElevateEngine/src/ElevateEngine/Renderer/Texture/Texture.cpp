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
		// TODO : PUT IN THE ORIGINAL STATE
		// THIS CODE IF FOR DEBUG ONLY LOADING ALL TEXTURES ASYNCHR.
		CreateAsync(path);

		//// TODO do for all other constructors / factory methods -> put in texture manager and set all stbi usage inside the texture manager
		//TexturePtr texture = TextureManager::GetTexture(path);
		//if (texture) return texture;

		//switch (Renderer::GetAPI())
		//{
		//case RendererAPI::GraphicAPI::None: texture = nullptr; break; // TODO implement
		//case RendererAPI::GraphicAPI::OpenGL: texture = std::make_shared<OpenGLTexture>(path);
		//}

		//return TextureManager::LoadTexture(texture);

		//EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
	}

	TexturePtr Texture::Create(char* data, int width, int height, int channelCount, std::string& path)
	{
		TexturePtr texture = TextureManager::GetTexture(path);
		if (texture) return texture;

		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: texture = nullptr; break; // TODO implement
		case RendererAPI::GraphicAPI::OpenGL: texture = std::make_shared<OpenGLTexture>(data, width, height, channelCount, path);
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

	TexturePtr Texture::CreateAsync(std::string& path)
	{
		TextureManager::LoadTextureAsync(path);
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
