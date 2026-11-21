module;

#include <memory>

export module Elevate.Editor.Renderer.ScenePreviewRenderer;

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
// Core
import Elevate.Core.Application;
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
		virtual ~ScenePreviewRenderer() = default;

		virtual void Render(const std::shared_ptr<Asset>& asset, Framebuffer& target)
		{
			if (!m_isSceneInitialized)
			{
				auto sceneManager = Application::Get().GetServices().Resolve<SceneManager>();
				InitScene(sceneManager);
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
		bool m_isSceneInitialized = false;
	};
}
