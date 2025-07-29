#include "eepch.h"
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

#include <ElevateEngine/Renderer/OpenGL/Texture/OpenGLTexture.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>
#include <filesystem>

#include <glm/common.hpp>

namespace Elevate
{
	bool Texture::MatchesPath(std::string pathToMatch)
	{
		std::filesystem::path currentPath = std::filesystem::absolute(m_meta.Path).lexically_normal();
		std::filesystem::path otherPath = std::filesystem::absolute(pathToMatch).lexically_normal();
		return currentPath == otherPath;
	}

	TexturePtr Texture::CreateFromFile(const std::string& path, TextureType usage)
	{
		return TextureManager::LoadTextureAsync(path);
	}

	TexturePtr Texture::CreateFromColor(const glm::vec3& color, const std::string& name, uint32_t width, uint32_t height)
	{
		size_t size = 3 * width * height;
		std::vector<unsigned char> pixels(size);

		unsigned char r = static_cast<unsigned char>(color.r);
		unsigned char g = static_cast<unsigned char>(color.g);
		unsigned char b = static_cast<unsigned char>(color.b);

		for (int i = 0; i < size; i += 3)
		{
			pixels[i + 0] = r;
			pixels[i + 1] = g;
			pixels[i + 2] = b;
		}

		TextureMetadata metaa = {
			name, "", width, height, 
			3, TextureFormat::RGB, TextureType::Diffuse, 
			TextureSource::Generated, TextureState::Loaded 
		};
		return CreateFromData(pixels.data(), metaa);
	}

	TexturePtr Texture::CreateFromColor(const glm::vec4& color, const std::string& name, uint32_t width, uint32_t height)
	{
		size_t size = 4 * width * height;
		std::vector<unsigned char> pixels(size);

		unsigned char r = static_cast<unsigned char>(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f);
		unsigned char g = static_cast<unsigned char>(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f);
		unsigned char b = static_cast<unsigned char>(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f);
		unsigned char a = static_cast<unsigned char>(glm::clamp(color.a, 0.0f, 1.0f) * 255.0f);

		for (int i = 0; i < size; i += 4)
		{
			pixels[i + 0] = r;
			pixels[i + 1] = g;
			pixels[i + 2] = b;
			pixels[i + 3] = a;
		}

		TextureMetadata meta = {
			name, "", width, height,
			4, TextureFormat::RGBA, TextureType::Diffuse,
			TextureSource::Generated, TextureState::Loaded
		};
		return CreateFromData(pixels.data(), meta);
	}

	TexturePtr Texture::CreateFromData(unsigned char* data, TextureMetadata& meta)
	{
		TexturePtr texture;
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: 
			texture = nullptr; 
			break;
		case RendererAPI::GraphicAPI::OpenGL: 
			texture = std::make_shared<OpenGLTexture>(data, meta); 
			break;
		default:
			EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
			break;
		}

		return TextureManager::RegisterTexture(texture);
	}
}
