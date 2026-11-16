module;

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <string>

export module Elevate.Editor.Scenes.MaterialPreviewScene;

// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Models;
import Elevate.Renderer.Meshes;
import Elevate.Renderer.Camera;
import Elevate.Renderer.Lights;
import Elevate.Renderer.Scene.Environment;
import Elevate.Renderer.Scene.Lighting;

export namespace Elevate
{
	class MaterialPreviewScene : public Scene
	{
	public:
		MaterialPreviewScene() = default;
		MaterialPreviewScene(SceneManager* sceneManager, std::string name, SceneType type = SceneType::RuntimeScene)
			: Scene(sceneManager, name, type)
		{
		}

		~MaterialPreviewScene() = default;

		virtual void Init() override
		{
			EmplaceContext<Elevate::SceneLighting>();
			auto env = EmplaceContext<Elevate::SceneEnvironment>();

			env.SetCubemap("engine://Cubemap/default.sky");

			auto sharedThis = std::static_pointer_cast<Scene>(shared_from_this());

			auto materialPreviewSphere = GameObject::Create(std::string(k_sphereName), sharedThis);
			materialPreviewSphere->AddComponent<Model>(PrimitiveType::UVSphere);

			auto materialPreviewCamera = GameObject::Create(std::string(k_cameraName), sharedThis);
			materialPreviewCamera->AddComponent<Camera>();
			materialPreviewCamera->SetPosition({ 0.0f, 3.0f, 5.0f });
			materialPreviewCamera->SetRotation({ -37.0f, 0.0f, 0.0f });

			auto dirLightObj = Elevate::GameObject::Create(std::string(k_dirLightName), sharedThis);
			auto& dirLight = dirLightObj->AddComponent<DirectionalLight>(glm::vec3(1.0f, 1.0f, 1.0f));
			dirLight.SetIntensity(0.5f);
			dirLightObj->SetPosition({ 7.0f, 4.0f, 6.0f });
			dirLightObj->SetRotation({ 0.0f, 45.0f, 45.0f });

			Scene::OnAwake();
		}

	private:
		static constexpr std::string_view k_sphereName = "MaterialPreview_Sphere";
		static constexpr std::string_view k_cameraName = "MaterialPreview_Camera";
		static constexpr std::string_view k_dirLightName = "MaterialPreview_DirLight";
	};
}
