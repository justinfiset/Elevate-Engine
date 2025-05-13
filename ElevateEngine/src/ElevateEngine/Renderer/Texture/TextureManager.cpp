#include "eepch.h"
#include "TextureManager.h"

#include "stb/stb_image.h"
#include <glad/glad.h>
#include <filesystem>

namespace Elevate {
	TexturePtr Elevate::TextureManager::LoadTexture(TexturePtr texture)
	{
		// TODO MAKE SURE TO HAVE THE ABSOLUTE PATH TO PREVENT MULTIPLE INCLUDES OR THE SAME TEXTURE WITH DIFF PATHS!!!
		if (instance().m_Textures.count(texture->GetPath()) > 0)
		{
			return instance().m_Textures[texture->GetPath()];
		}
		else
		{
			instance().m_Textures[texture->GetPath()] = texture;
			return texture;
		}
	}

	TexturePtr TextureManager::LoadTexture(std::string& path)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (!data)
		{
			EE_CORE_ERROR("Failed to load texture {}", path);
			return nullptr;
		}

		auto texture = LoadTexture(Texture::Create(data, width, height, channels, path));
		stbi_image_free(data);
		return texture;
	}

	TexturePtr TextureManager::GetTexture(const std::string& path)
	{
		std::filesystem::path fsPath = std::filesystem::absolute(path);
		return (instance().m_Textures.count(fsPath.string()) > 0) ? instance().m_Textures[fsPath.string()] : nullptr;
	}

	TexturePtr TextureManager::LoadTextureAsync(const std::string& path)
	{
		std::filesystem::path fsPath = std::filesystem::absolute(path);
		std::string absPath = fsPath.string();

		// If the texture is already loading, or already loaded, return and cancel
		if (!GetTexture(absPath))
		{
			for (TextureLoadResult& res : instance().m_loadingTextures) {
				if (res.path == absPath)
				{
					return nullptr;
				}
			}
		}

		// Creation of a black image
		TexturePtr tex = Texture::Create(nullptr, 0, 0, 0, absPath);
		instance().m_Textures[path] = tex;

		TextureLoadResult res;

		res.path = absPath; // Store the absolute path in the result (security for the map later on)

		// Get the texture data async with stbi_load
		std::thread([res]() mutable {
				res.data = stbi_load(res.path.c_str(), &res.width, &res.height, &res.channelsCount, STBI_rgb_alpha);

				std::lock_guard<std::mutex> lock(instance().m_textureMutex);
				res.loaded = (res.data != nullptr); // Loaded if the texture is not null
				instance().m_loadingTextures.push_back(res);
			}
		).detach();

		return tex;
	}

	void TextureManager::UpdateLoadingTextures()
	{
		TextureManager& manager = instance();
		std::lock_guard<std::mutex> lock(manager.m_textureMutex);

		auto it = manager.m_loadingTextures.begin();
		while (it != manager.m_loadingTextures.end())
		{
			if (it->loaded)
			{
				EE_CORE_TRACE(it->path);
				// Mise à jour de la texture existante plutôt que création d'une nouvelle
				if (manager.m_Textures.count(it->path))
				{
					manager.m_Textures[it->path]->SetData(it->data, it->width, it->height, it->channelsCount);
					stbi_image_free(it->data);
					it->data = nullptr;
				}
				it = manager.m_loadingTextures.erase(it);  // Suppression sécurisée
			}
			else
			{
				++it;
			}
		}	
	}
}