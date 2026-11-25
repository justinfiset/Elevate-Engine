module;

#include <vector>
#include <unordered_map>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

export module Elevate.Renderer.Texture:TextureManager;

import Elevate.Foundations.Services;
import Elevate.Foundations.Tasks;

import :TextureMetadata;
import :Texture;

export namespace Elevate
{
	struct TextureLoadResult
	{
		unsigned char* data;
		TextureMetadata meta;
	};

	class TextureManager
	{
	public:
		static std::shared_ptr<Texture> RegisterTexture(std::shared_ptr<Texture> texture);

		static std::shared_ptr<Texture> GetTexture(const std::string& path);
		static std::shared_ptr<Texture> LoadTextureAsync(const std::string& path, TextureType usage = TextureType::Diffuse);
		static std::shared_ptr<Texture> LoadTextureAsync(const std::string& path, const TextureMetadata& metadata);

		inline static std::shared_ptr<Texture> GetDefaultTexture() { return instance().m_defaultTexture; }

		inline static bool IsAllLoaded() { return instance().m_loadingTextures.empty(); }

		static void UpdateLoadingTextures();

	private:
		static std::shared_ptr<Texture> LoadTextureAsyncInternal(const std::string& path, TextureMetadata metadata);

		struct TextureLoadTask {
			std::string path;
			TextureMetadata meta;
		};

		TextureManager();
		~TextureManager();

		void LoadTextureTask(TextureMetadata meta);

		static TextureManager& instance()
		{
			static TextureManager instance;
			return instance;
		}

		std::shared_ptr<Texture> m_defaultTexture;

		// Async loading & Thread Pool
		std::vector<TextureLoadResult> m_loadingTextures;
		std::mutex m_textureMutex;

		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	};
}