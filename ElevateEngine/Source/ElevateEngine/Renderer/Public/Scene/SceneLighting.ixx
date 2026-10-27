module;

#include <initializer_list>
#include <memory>
#include <span>
#include <vector>

#include <glm/mat4x4.hpp>

export module Elevate.Renderer.Scene.Lighting;

// Foundations
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Lights.DirectionalLight;
import Elevate.Renderer.Lights.PointLight;
import Elevate.Renderer.Lights.SpotLight;

export namespace Elevate::SceneLightingLimits
{
	inline constexpr int MaxDirectionalLights = 1;
	inline constexpr int MaxPointLights = 32;
	inline constexpr int MaxSpotLights = 32;
}

export namespace Elevate
{
	class SceneLighting
	{
	public:
		SceneLighting() = default;
		SceneLighting(DirectionalLight* dirLight, std::initializer_list<PointLight*> pointLights, std::initializer_list<SpotLight*> spotLights)
			: m_dirLight(dirLight), m_pointLights(pointLights), m_spotLights(spotLights)
		{
		}
		SceneLighting(DirectionalLight* dirLight, const std::vector<PointLight*>& pointLights, const std::vector<SpotLight*>& spotLights)
			: m_dirLight(dirLight), m_pointLights(pointLights), m_spotLights(spotLights)
		{
		}

		void UploadToShader(const std::shared_ptr<Shader>& shader) const;

		const DirectionalLight* GetDirLight() const;

		void AddDirectionalLight(DirectionalLight* light);
		void AddPointLight(PointLight* light);
		void AddSpotLight(SpotLight* light);

		void RemoveDirectionalLight(DirectionalLight* light);
		void RemovePointLight(PointLight* light);
		void RemoveSpotLight(SpotLight* light);

		glm::mat4 GetDirectionalLightSpaceMatrix(const std::span<const glm::vec3> cameraCorners) const;

	private:
		glm::mat4 GetDirectionalLightProjection(const std::span<const glm::vec3> corners, const glm::mat4& lightView) const;

		DirectionalLight* m_dirLight = nullptr;
		std::vector<PointLight*> m_pointLights;
		std::vector<SpotLight*> m_spotLights;
	};
}

export namespace Elevate
{
	SceneLighting* GetSceneLighting(Scene* scene)
	{
		if (!scene)
		{
			CoreLogger::Warn("GetSceneLighting: Scene is null.");
			return nullptr;
		}

		return scene->TryGetContext<SceneLighting>();
	}
}
