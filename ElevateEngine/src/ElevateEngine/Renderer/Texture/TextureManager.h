#pragma once
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <unordered_map>

namespace Elevate 
{
	struct TextureLoadResult
	{
		unsigned char* data;
		TextureMetadata meta;
	};

	class TextureManager
	{
	public:
		static TexturePtr RegisterTexture(TexturePtr texture);

		static TexturePtr GetTexture(const std::string& path);
		static TexturePtr LoadTextureAsync(const std::string& path);

		inline static TexturePtr GetDefaultTexture() { return  instance().m_defaultTexture; }

		inline static bool IsAllLoaded() { return instance().m_loadingTextures.empty(); }

		friend class Application;
	protected:
		static void UpdateLoadingTextures();
	private:
		TextureManager();

		static TextureManager& instance()
		{
			static TextureManager instance;
			return instance;
		}

		TexturePtr m_defaultTexture;

		// Async loading
		std::vector<TextureLoadResult> m_loadingTextures;
		std::mutex m_textureMutex;

		std::unordered_map<std::string, TexturePtr> m_Textures;
	};
}