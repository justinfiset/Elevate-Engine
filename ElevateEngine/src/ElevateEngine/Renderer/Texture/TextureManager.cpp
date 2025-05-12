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

	TexturePtr TextureManager::LoadTexture(std::string path)
	{
		return LoadTexture(Texture::Create(path));
	}

	TexturePtr TextureManager::GetTexture(std::string path)
	{
		std::filesystem::path fsPath = std::filesystem::absolute(path);
		return (instance().m_Textures.count(fsPath.string()) > 0) ? instance().m_Textures[fsPath.string()] : nullptr;
	}

	void TextureManager::LoadTextureAsync(const std::string& path)
	{
		// TODO: PREVENT ALREADY LOADED TEXTURE TO LOAD ASYNC AGAIN -> INSTEAD RETURN DIRECTLY THE NEW TEXTURE SOMEHOW
		TextureLoadResult res;
		std::filesystem::path fsPath = std::filesystem::absolute(path);
		res.path = fsPath.string(); // Store the absolute path in the result (security for the map later on)

		// Get the texture data async with stbi_load
		std::thread([res]() mutable {
			res.data = stbi_load(res.path.c_str(), &res.width, &res.height, &res.channelsCount, STBI_rgb_alpha);

			std::lock_guard<std::mutex> lock(instance().m_textureMutex);
			res.loaded = (res.data != nullptr); // Loaded if the texture is not null
			instance().m_loadingTextures.push_back(res);
		}).detach();
	}

	void TextureManager::UpdateLoadingTextures()
	{
		std::lock_guard<std::mutex> lock(m_textureMutex);

		std::vector<std::string> loadedTextures;
		for (TextureLoadResult& res : m_loadingTextures) 
		{
			if (res.loaded && res.textureID != 0) 
			{
				TexturePtr tex = Texture::Create(res.data, res.width, res.height, res.channelsCount, res.path);
				stbi_image_free(res.data);
				res.data = nullptr;
			}
		}
	}
}