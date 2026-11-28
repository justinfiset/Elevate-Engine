module;

#include <initializer_list>
#include <memory>
#include <span>
#include <vector>

#include <glm/mat4x4.hpp>

export module Elevate.Renderer.Scene.Lighting;

export import Elevate.Renderer.Scene.Lighting.Limits;

// Foundations
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Lights.Contract;

export namespace Elevate
{
	class SceneLighting
	{
	public:
		using ShaderBinder = bool (*)(const std::shared_ptr<Shader>&);

		static void SetShaderBinder(ShaderBinder binder);

		SceneLighting() = default;
		SceneLighting(IDirectionalLight* dirLight, std::initializer_list<IPointLight*> pointLights, std::initializer_list<ISpotLight*> spotLights)
			: m_dirLight(dirLight), m_pointLights(pointLights), m_spotLights(spotLights)
		{
		}
		SceneLighting(IDirectionalLight* dirLight, const std::vector<IPointLight*>& pointLights, const std::vector<ISpotLight*>& spotLights)
			: m_dirLight(dirLight), m_pointLights(pointLights), m_spotLights(spotLights)
		{
		}

		void UploadToShader(const std::shared_ptr<Shader>& shader) const;

		const IDirectionalLight* GetDirLight() const;

		void AddDirectionalLight(IDirectionalLight* light);
		void AddPointLight(IPointLight* light);
		void AddSpotLight(ISpotLight* light);

		void RemoveDirectionalLight(IDirectionalLight* light);
		void RemovePointLight(IPointLight* light);
		void RemoveSpotLight(ISpotLight* light);

		glm::mat4 GetDirectionalLightSpaceMatrix(const std::span<const glm::vec3> cameraCorners) const;

	private:
		static ShaderBinder s_shaderBinder;

		glm::mat4 GetDirectionalLightProjection(const std::span<const glm::vec3> corners, const glm::mat4& lightView) const;

		IDirectionalLight* m_dirLight = nullptr;
		std::vector<IPointLight*> m_pointLights;
		std::vector<ISpotLight*> m_spotLights;
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
