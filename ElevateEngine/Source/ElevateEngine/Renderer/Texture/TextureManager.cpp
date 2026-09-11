#include "TextureManager.h"

#include <filesystem>
#include <stb/stb_image.h>
#include <ElevateEngine/Renderer/GraphicsAPI.h>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/PathResolver.h>

namespace Elevate {
    TextureManager::TextureManager()
    {
        m_defaultTexture = Texture::CreateFromColor({ 1.0, 1.0, 1.0, 1.0 }, "default");

        const size_t workerCount = 2;
        m_workerThreads.reserve(workerCount);
        for (size_t i = 0; i < workerCount; ++i) {
            m_workerThreads.emplace_back(&TextureManager::WorkerLoop, this);
        }

        EE_CORE_INFO("Texture Manager Initialized with {} worker thread(s).", workerCount);
    }

    TextureManager::~TextureManager()
    {
        m_isWorkerRunning = false;
        m_queueCV.notify_all();
        for (auto& t : m_workerThreads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    void TextureManager::WorkerLoop()
    {
        while (m_isWorkerRunning) {
            TextureLoadTask task;
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_queueCV.wait(lock, [this] {
                    return !m_loadQueue.empty() || !m_isWorkerRunning;
                    });

                if (!m_isWorkerRunning && m_loadQueue.empty()) {
                    return;
                }

                task = std::move(m_loadQueue.front());
                m_loadQueue.pop();
            }

            int width = 0;
            int height = 0;
            int channels = 0;
            unsigned char* data = stbi_load(task.meta.Path.c_str(), &width, &height, &channels, 0);

            TextureFormat format = TextureFormat::RGBA;
            switch (channels)
            {
            case 1: format = TextureFormat::GRAYSCALE; break;
            case 3: format = TextureFormat::RGB;       break;
            case 4: format = TextureFormat::RGBA;      break;
            }

            TextureLoadResult res;
            res.meta = TextureMetadataBuilder(task.meta)
                .size(static_cast<uint32_t>(width), static_cast<uint32_t>(height))
                .Format(format)
                .State((data) ? TextureState::Loaded : TextureState::Failed)
                .Build();
            res.data = data;

            {
                std::lock_guard<std::mutex> lock(instance().m_textureMutex);
                instance().m_loadingTextures.push_back(res);
            }
        }
    }

    TexturePtr Elevate::TextureManager::RegisterTexture(TexturePtr texture)
    {
        std::string key;

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

    TexturePtr TextureManager::GetTexture(const std::string& path)
    {
        std::filesystem::path fsPath = std::filesystem::absolute(path);
        return (instance().m_Textures.count(fsPath.string()) > 0) ? instance().m_Textures[fsPath.string()] : nullptr;
    }

    TexturePtr TextureManager::LoadTextureAsync(const std::string& path, TextureType usage)
    {
        std::string resolvedPath = PathResolver::Resolve(path);
        std::filesystem::path fsPath = std::filesystem::absolute(resolvedPath);
        std::string absPath = fsPath.string();

        TexturePtr tex = GetTexture(absPath);
        if (!tex)
        {
            for (TextureLoadResult& res : instance().m_loadingTextures) {
                if (res.meta.Path == absPath)
                {
                    return nullptr;
                }
            }
        }
        else
        {
            return tex;
        }

        TextureMetadata meta = TextureMetadataBuilder()
            .Name(fsPath.filename().string())
            .Path(absPath)
            .size(0, 0)
            .Format(TextureFormat::EMPTY)
            .Usage(usage)
            .Source(TextureSource::File)
            .State(TextureState::Unloaded)
            .Build();

        tex = Texture::CreateFromData(nullptr, meta);
        instance().m_Textures[absPath] = tex;

        {
            std::lock_guard<std::mutex> lock(instance().m_queueMutex);
            instance().m_loadQueue.push({ absPath, meta });
        }

        instance().m_queueCV.notify_one();

        return tex;
    }

    void TextureManager::UpdateLoadingTextures()
    {
        TextureManager& manager = instance();
        std::lock_guard<std::mutex> lock(manager.m_textureMutex);

        auto it = manager.m_loadingTextures.begin();
        while (it != manager.m_loadingTextures.end())
        {
            switch (it->meta.State)
            {
            case TextureState::Loaded:
                if (manager.m_Textures.count(it->meta.Path))
                {
                    EE_CORE_INFO("Loaded texture : {}, {}x{}", it->meta.Path, it->meta.Width, it->meta.Height);
                    manager.m_Textures[it->meta.Path]->SetData(it->data, it->meta);
                    stbi_image_free(it->data);
                    it->data = nullptr;
                }
                it = manager.m_loadingTextures.erase(it);
                break;
            case TextureState::Failed:
                if (manager.m_Textures.count(it->meta.Path))
                {
                    EE_CORE_ERROR("Failed to load texture : {}", it->meta.Path);
                    manager.m_Textures[it->meta.Path]->SetData(it->data, it->meta);
                    if (it->data)
                    {
                        stbi_image_free(it->data);
                    }
                    it->data = nullptr;
                }
                it = manager.m_loadingTextures.erase(it);
                break;
            default:
                ++it;
                break;
            }
        }
    }
}