#pragma once
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <unordered_map>

namespace Elevate 
{
	struct TextureLoadResult
	{
		unsigned char* data;
		int width, height, channelsCount;
		std::string path;
		bool loaded = false;
	};

	class TextureManager
	{
	public:
		static TexturePtr LoadTexture(TexturePtr texture);
		static TexturePtr LoadTexture(std::string& path);

		static TexturePtr GetTexture(const std::string& path);
		static TexturePtr LoadTextureAsync(const std::string& path);

		inline static bool IsAllLoaded() { return instance().m_loadingTextures.empty(); }

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

		std::unordered_map<std::string, TexturePtr> m_Textures;
	};
}