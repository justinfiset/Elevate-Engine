module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <memory>

export module Elevate.Editor.Renderer.ScenePreviewRenderer;

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
// Core
import Elevate.Core.Scene;
import Elevate.Core.Assets.Asset;
// Renderer
import Elevate.Renderer.Renderer;
import Elevate.Renderer.Camera;
import Elevate.Renderer.Framebuffers;
// Editor
import Elevate.Editor.Renderer.AssetPreviewRenderer;

export namespace Elevate
{
	class ScenePreviewRenderer : public AssetPreviewRenderer
	{
	public:
		using SceneManagerProvider = std::shared_ptr<SceneManager> (*)();

		virtual ~ScenePreviewRenderer() = default;

		static void SetSceneManagerProvider(SceneManagerProvider provider)
		{
			s_sceneManagerProvider = provider;
		}

		virtual void Render(const std::shared_ptr<Asset>& asset, Framebuffer& target)
		{
			if (!m_isSceneInitialized)
			{
				if (!s_sceneManagerProvider)
				{
					CoreLogger::Error("ScenePreviewRenderer: SceneManager provider is not set.");
					return;
				}

				InitScene(s_sceneManagerProvider());
				m_isSceneInitialized = true;
			}

			if (!m_scene)
			{
				CoreLogger::Error("ScenePreviewRenderer: Scene is not initialized.");
				return;
			}

			UseAsset(asset);

			Renderer::BeginFrame(m_scene, GetCamera(), &target);
			m_scene->RenderScene();
			Renderer::RenderFrame();
		}

	protected:
		virtual void InitScene(SharedService<SceneManager> sceneManager) = 0;
		virtual void UseAsset(const std::shared_ptr<Asset>& asset) = 0;
		virtual Camera& GetCamera() = 0;

		std::shared_ptr<Scene> m_scene;

	private:
		inline static SceneManagerProvider s_sceneManagerProvider = nullptr;
		bool m_isSceneInitialized = false;
	};
}
