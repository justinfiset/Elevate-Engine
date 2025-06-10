#include "eepch.h"
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

#include <ElevateEngine/Renderer/OpenGL/Texture/OpenGLTexture.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>
#include <filesystem>

namespace Elevate
{
	TexturePtr Texture::Create(const std::string& path)
	{
		//// TODO do for all other constructors / factory methods -> put in texture manager and set all stbi usage inside the texture manager
		TexturePtr texture = TextureManager::GetTexture(path);
		if (texture) return texture;
		
		// TODO : PUT IN THE ORIGINAL STATE
		// THIS CODE IF FOR DEBUG ONLY LOADING ALL TEXTURES ASYNCHR.
		return CreateAsync(path);
		//switch (Renderer::GetAPI())
		//{
		//case RendererAPI::GraphicAPI::None: texture = nullptr; break; // TODO implement
		//case RendererAPI::GraphicAPI::OpenGL: texture = std::make_shared<OpenGLTexture>(path);
		//}

		//return TextureManager::LoadTexture(texture);

		//EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
	}

	TexturePtr Texture::Create(unsigned char* data, int width, int height, int channelCount, const std::string& path)
	{
		TexturePtr texture = TextureManager::GetTexture(path);
		if (texture) return texture;

		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: texture = nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: texture = std::make_shared<OpenGLTexture>(data, width, height, channelCount, path);
		}

		if (texture)
		{
			return TextureManager::LoadTexture(texture);
		}
		else 
		{
			EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
		}
	}

	TexturePtr Texture::Create(const std::string& path, unsigned int index, const std::string type)
	{
		TexturePtr tex = nullptr;

		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: tex = nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: tex = std::make_shared<OpenGLTexture>(path, index, type);
		}

		if (tex)
		{
			return tex;
		}
		else
		{
			EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
		}
	}

	TexturePtr Texture::CreateAsync(const std::string& path)
	{
		return TextureManager::LoadTextureAsync(path);
	}

	bool Texture::MatchesPath(std::string pathToMatch)
	{
		std::filesystem::path otherPath = pathToMatch;
		if (otherPath.is_absolute()) {
			return pathToMatch == this->m_path;
		}
		else {
			return std::filesystem::absolute(otherPath).string() == this->m_path;
		}
	}
}
