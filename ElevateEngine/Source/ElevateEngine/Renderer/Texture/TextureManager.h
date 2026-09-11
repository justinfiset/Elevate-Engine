#pragma once

#include <vector>
#include <unordered_map>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

#include <ElevateEngine/Renderer/Texture/Texture.h>

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
        static TexturePtr LoadTextureAsync(const std::string& path, TextureType usage = TextureType::Diffuse);

        inline static TexturePtr GetDefaultTexture() { return instance().m_defaultTexture; }

        inline static bool IsAllLoaded() { return instance().m_loadingTextures.empty(); }

        friend class Application;
    protected:
        static void UpdateLoadingTextures();

    private:
        struct TextureLoadTask {
            std::string path;
            TextureMetadata meta;
        };

        TextureManager();
        ~TextureManager();

        void WorkerLoop();

        static TextureManager& instance()
        {
            static TextureManager instance;
            return instance;
        }

        TexturePtr m_defaultTexture;

        // Async loading & Thread Pool
        std::vector<TextureLoadResult> m_loadingTextures;
        std::mutex m_textureMutex;

        std::queue<TextureLoadTask> m_loadQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_queueCV;
        std::vector<std::thread> m_workerThreads;
        std::atomic<bool> m_isWorkerRunning{ true };

        std::unordered_map<std::string, TexturePtr> m_Textures;
    };
}