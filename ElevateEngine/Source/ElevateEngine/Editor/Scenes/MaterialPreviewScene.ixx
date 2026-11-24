module;

#include <ElevateEngine/Core/Scene/SceneEnTTOperators.h>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <string>
#include <cmath>

export module Elevate.Editor.Scenes.MaterialPreviewScene;

// Foundations
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Materials.Material;
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
		MaterialPreviewScene(SceneManager* sceneManager, std::string name, SceneType type = SceneType::RuntimeScene)
			: Scene(sceneManager, std::move(name), type)
		{
		}

		~MaterialPreviewScene() override = default;

		virtual void Init() override
		{
			EmplaceContext<Elevate::SceneLighting>();
			auto& env = EmplaceContext<Elevate::SceneEnvironment>();

			env.SetCubemap("engine://Cubemap/default.sky");

			const float distance = (k_radius / std::sin(glm::radians(k_fovDeg * 0.5f))) * k_margin;
			const float pitch = glm::radians(k_pitchDeg);

			auto sharedThis = std::static_pointer_cast<Scene>(shared_from_this());

			auto materialPreviewSphere = GameObject::Create(std::string(k_sphereName), sharedThis);
			m_previewModel = &materialPreviewSphere->AddComponent<Model>(PrimitiveType::UVSphere);

			auto materialPreviewCamera = GameObject::Create(std::string(k_cameraName), sharedThis);
			m_camera = &materialPreviewCamera->AddComponent<Camera>();
			materialPreviewCamera->SetPosition({ 0.0f, distance * std::sin(pitch), distance * std::cos(pitch) });
			materialPreviewCamera->SetRotation({ -k_pitchDeg, 0.0f, 0.0f });

			const float lp = glm::radians(k_lightPitchDeg);
			const float ly = glm::radians(k_lightYawDeg);

			const glm::vec3 lightPos = glm::vec3(
				std::cos(lp) * std::sin(ly),
				-std::sin(lp),
				std::cos(lp) * std::cos(ly)
			) * k_lightDistance;

			auto dirLightObj = GameObject::Create(std::string(k_dirLightName), sharedThis);
			auto& dirLight = dirLightObj->AddComponent<DirectionalLight>(glm::vec3(1.0f, 1.0f, 1.0f));
			dirLight.SetIntensity(0.5f);
			dirLightObj->SetPosition(lightPos);
			dirLightObj->SetRotation({ k_lightPitchDeg, k_lightYawDeg, 0.0f });

			auto ground = GameObject::Create(std::string(k_groundName), sharedThis);
			ground->AddComponent<Model>(PrimitiveType::Plane);
			ground->SetPosition({ 0.0f, -k_radius, 0.0f });
			ground->SetScale({ k_groundSize, 1.0f, k_groundSize });
		}

		void SetPreviewMaterial(const std::shared_ptr<Material>& material)
		{
			if (!m_previewModel)
			{
				CoreLogger::Warn("Cannot set a material to preview with a non initialized preview scene.");
				return;
			}

			m_previewModel->SetMaterial(material);
		}

		Camera* GetCamera()
		{
			return m_camera;
		}

	private:
		Camera* m_camera = nullptr;
		Model* m_previewModel = nullptr;

		static constexpr std::string_view k_sphereName = "MaterialPreview_Sphere";
		static constexpr std::string_view k_cameraName = "MaterialPreview_Camera";
		static constexpr std::string_view k_dirLightName = "MaterialPreview_DirLight";
		static constexpr std::string_view k_groundName = "MaterialPreview_Ground";

		static constexpr float k_groundSize = 50.0f;

		static constexpr float k_fovDeg = 60.0f;
		static constexpr float k_radius = 1.0f;
		static constexpr float k_margin = 1.25f;
		static constexpr float k_pitchDeg = 15.0f;

		static constexpr float k_lightPitchDeg = -40.0f;
		static constexpr float k_lightYawDeg = -30.0f;
		static constexpr float k_lightDistance = 6.0f;
	};
}
