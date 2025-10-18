#include "eepch.h"
#include "TextureManager.h"

#include "stb/stb_image.h"
#include <glad/glad.h>
#include <filesystem>
#include <ElevateEngine/Core/Log.h>

namespace Elevate {
	TextureManager::TextureManager()
	{
		//TODO : UNCOMMENT AND IMPL 
		m_defaultTexture = Texture::CreateFromColor({ 1.0,1.0,1.0,1.0 }, "default");
		EE_CORE_INFO("Texture Manager Initialized.");
	}

	TexturePtr Elevate::TextureManager::RegisterTexture(TexturePtr texture)
	{
		std::string key;

		// TODO FIN A WAY TO GET THE ABOSLUTE PATH AND TO NORMALIZE IT
		if (texture->GetMetadata().Source == TextureSource::File)
		{
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
		else return texture;
	}

	// TODO REMOVE OR IMPL. BUT WE NEED TO DO SOMETHING WITH IT
	/// FUNCTION TO LOAD AND GET A TEXTURE SYNCED WITH THE CURRENT THREAD
	//TexturePtr TextureManager::RegisterTexture(std::string& path)
	//{
	//	int width, height, channels;
	//	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	//	if (!data)
	//	{
	//		EE_CORE_ERROR("Failed to load texture {}", path);
	//		return nullptr;
	//	}

	//	auto texture = AddOrGetTexture(Texture::CreateFromData(data, width, height, (TextureFormat) channels, path));
	//	stbi_image_free(data);
	//	return texture;
	//}

	TexturePtr TextureManager::GetTexture(const std::string& path)
	{
		std::filesystem::path fsPath = std::filesystem::absolute(path);
		return (instance().m_Textures.count(fsPath.string()) > 0) ? instance().m_Textures[fsPath.string()] : nullptr;
	}

	TexturePtr TextureManager::LoadTextureAsync(const std::string& path)
	{
		std::filesystem::path fsPath = std::filesystem::absolute(path);
		std::string absPath = fsPath.string();

		EE_CORE_TRACE("{}", absPath);

		// If the texture is already loading, or already loaded, return and cancel
		TexturePtr tex = GetTexture(absPath);
		if (!tex)
		{
			for (TextureLoadResult& res : instance().m_loadingTextures) {
				if (res.meta.Path == absPath)
				{
					// todo: change, should return an empty texture placeholder
					return nullptr;
				}
			}
		}
		else
		{
			return tex;
		}

		// Creation of a blank texture
		TextureMetadata meta = TextureMetadataBuilder()
			.Name(fsPath.filename().string())
			.Path(absPath)
			.Size(0, 0)
			.Format(TextureFormat::EMPTY)
			.Usage(TextureType::Diffuse)
			.Source(TextureSource::File)
			.State(TextureState::Unloaded)
			.Build();

		tex = Texture::CreateFromData(nullptr, meta);

		// Add the texture to the list of textures
		instance().m_Textures[absPath] = tex;

		TextureLoadResult res;
		res.meta = meta;

		// Get the texture data async with stbi_load
		std::thread([res]() mutable {
				int width, height, channels;
				res.data = stbi_load(res.meta.Path.c_str(), &width, &height, &channels, 0);

				res.meta = TextureMetadataBuilder(res.meta)
					.Size(static_cast<uint32_t>(width), static_cast<uint32_t>(height))
					.Format((TextureFormat) channels)
					.State((res.data != nullptr) ? TextureState::Loaded : TextureState::Failed)
					.Build();

				std::lock_guard<std::mutex> lock(instance().m_textureMutex);
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
			if (it->meta.State == TextureState::Loaded)
			{
				if (manager.m_Textures.count(it->meta.Path))
				{
					EE_CORE_INFO("{}", it->meta.Path);
					manager.m_Textures[it->meta.Path]->SetData(it->data, it->meta);
					stbi_image_free(it->data);
					it->data = nullptr;
				}
				it = manager.m_loadingTextures.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}