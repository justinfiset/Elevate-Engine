#pragma once
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <unordered_map>

namespace Elevate 
{
	class TextureManager
	{
	public:
		static TexturePtr LoadTexture(TexturePtr texture);
		static TexturePtr LoadTexture(std::string& path);

		static TexturePtr GetTexture(std::string& path);
		static void LoadTextureAsync(const std::string& path);

		friend class Application;
	protected:
		static void UpdateLoadingTextures();
	private:
		TextureManager() = default;

		static TextureManager& instance()
		{
			static TextureManager instance;
			return instance;
		}

		// Async loading
		std::vector<TextureLoadResult> m_loadingTextures;
		std::mutex m_textureMutex;
		uint32_t m_iterCount;

		std::unordered_map<std::string, TexturePtr> m_Textures;
	};
}