module;

#include <mutex>
#include <memory>
#include <filesystem>
#include <stb/stb_image.h>

#include <ElevateEngine/Renderer/GraphicsAPI.h>

module Elevate.Renderer.Texture;

import Elevate.Foundations.Paths;
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
import Elevate.Foundations.Tasks;

import Elevate.Core.Application; // Used to resolve services

namespace Elevate {
	TextureManager::TextureManager()
	{
		m_defaultTexture = Texture::CreateFromColor({ 1.0, 1.0, 1.0, 1.0 }, "default");

		CoreLogger::Info("Texture Manager Initialized.");
	}

	TextureManager::~TextureManager()
	{
	}

	void TextureManager::LoadTextureTask(TextureMetadata meta)
	{
		int width = 0;
		int height = 0;
		int channels = 0;

		unsigned char* data = stbi_load(
			meta.Path.c_str(),
			&width,
			&height,
			&channels,
			0
		);

		TextureFormat format = TextureFormat::RGBA;

		switch (channels)
		{
		case 1: format = TextureFormat::GRAYSCALE; break;
		case 3: format = TextureFormat::RGB; break;
		case 4: format = TextureFormat::RGBA; break;
		}

		TextureLoadResult result;
		result.meta = TextureMetadataBuilder(meta)
			.size(width, height)
			.Format(format)
			.State(data ? TextureState::Loaded : TextureState::Failed)
			.Build();

		result.data = data;
		TextureManager& manager = instance();

		{
			std::lock_guard lock(manager.m_textureMutex);
			manager.m_loadingTextures.push_back(std::move(result));
		}
	}

	std::shared_ptr<Texture> TextureManager::RegisterTexture(std::shared_ptr<Texture> texture)
	{
		if (texture->GetMetadata().Source != TextureSource::File)
		{
			return texture;
		}

		TextureManager& manager = instance();
		std::lock_guard lock(manager.m_textureMutex);

		auto [it, inserted] = manager.m_Textures.try_emplace(texture->GetPath(), texture);
		return it->second;
	}

	std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& path)
	{
		TextureManager& manager = instance();
		std::lock_guard lock(manager.m_textureMutex);

		std::filesystem::path fsPath = std::filesystem::absolute(path);
		auto it = manager.m_Textures.find(fsPath.string());

		return it != manager.m_Textures.end() ? it->second : nullptr;
	}

	std::shared_ptr<Texture> TextureManager::LoadTextureAsync(const std::string& path, TextureType usage)
	{
		TextureMetadata meta = TextureMetadataBuilder()
			.Name(std::filesystem::path(path).filename().string())
			.Usage(usage)
			.Source(TextureSource::File)
			.Build();

		return LoadTextureAsyncInternal(path, std::move(meta));
	}

	std::shared_ptr<Texture> TextureManager::LoadTextureAsync(const std::string& path, const TextureMetadata& metadata)
	{
		return LoadTextureAsyncInternal(path, TextureMetadata(metadata));
	}

	std::shared_ptr<Texture> TextureManager::LoadTextureAsyncInternal(const std::string& path, TextureMetadata meta)
	{
		auto resolvedPath = PathResolver::Resolve(path);
		std::filesystem::path fsPath = std::filesystem::absolute(resolvedPath);
		std::string absPath = fsPath.string();

		meta.Name = fsPath.filename().string();
		meta.Path = absPath;
		meta.State = TextureState::Unloaded;

		TextureManager& manager = instance();

		{
			std::lock_guard lock(manager.m_textureMutex);

			auto it = manager.m_Textures.find(absPath);
			if (it != manager.m_Textures.end())
			{
				return it->second;
			}
		}

		auto texture = Texture::CreateFromData(nullptr, meta);

		Application::Get().GetServices().Resolve<ITaskScheduler>()->Submit(
			TaskDescriptor(TaskAffinity::Any, TaskPriority::Normal, "LoadTexture" + path),
			[meta = std::move(meta)]() mutable
			{
				instance().LoadTextureTask(std::move(meta));
			});

		return texture;
	}

	void TextureManager::UpdateLoadingTextures()
	{
		std::vector<TextureLoadResult> results;
		TextureManager& manager = instance();

		{
			std::lock_guard lock(manager.m_textureMutex);
			results.swap(manager.m_loadingTextures);
		}

		for (auto& result : results)
		{
			std::shared_ptr<Texture> texture;

			{
				std::lock_guard lock(manager.m_textureMutex);

				auto it = manager.m_Textures.find(result.meta.Path);
				if (it != manager.m_Textures.end())
				{
					texture = it->second;
				}
			}

			if (texture)
			{
				if (result.meta.State == TextureState::Loaded)
				{
					CoreLogger::Info(
						"Loaded texture : {}, {}x{}",
						result.meta.Path,
						result.meta.Width,
						result.meta.Height
					);
				}
				else
				{
					CoreLogger::Error(
						"Failed to load texture : {}",
						result.meta.Path
					);
				}

				texture->SetData(result.data, result.meta);
			}

			if (result.data)
			{
				stbi_image_free(result.data);
			}
		}
	}
}