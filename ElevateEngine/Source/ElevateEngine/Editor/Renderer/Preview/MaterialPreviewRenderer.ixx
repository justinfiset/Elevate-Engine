module;


#include <memory>

export module Elevate.Editor.Renderer.MaterialPreviewRenderer;

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
// Core
import Elevate.Core.Scene;
import Elevate.Core.Assets.Asset;
// Renderer
import Elevate.Renderer.Materials.Material;
import Elevate.Renderer.Framebuffers;
import Elevate.Renderer.Camera;
// Editor
import Elevate.Editor.Renderer.ScenePreviewRenderer;
import Elevate.Editor.Scenes.MaterialPreviewScene;

export namespace Elevate
{
	class MaterialPreviewRenderer : public ScenePreviewRenderer
	{
	public:
		virtual ~MaterialPreviewRenderer() = default;

	private:
		std::shared_ptr<MaterialPreviewScene> m_previewScene = nullptr;

		void InitScene(SharedService<SceneManager> sceneManager) override
		{
			m_scene = sceneManager->CreateScene<MaterialPreviewScene>("Material Preview Scene", SceneType::EditorScene);
			m_previewScene = std::static_pointer_cast<MaterialPreviewScene>(m_scene);
		}

		void UseAsset(const std::shared_ptr<Asset>& asset) override
		{
			auto material = std::static_pointer_cast<Material>(asset);
			m_previewScene->SetPreviewMaterial(material);
		}

		Camera& GetCamera() override
		{
			return *m_previewScene->GetCamera();
		}
	};
}
