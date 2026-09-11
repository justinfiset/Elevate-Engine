#pragma once

#include <memory>
#include <vector>
#include <span>

#include <glm/mat4x4.hpp>

// The maximum number of each lights to have in the game, these numbers are passed to the shader at compile time
#define MAX_DIRECTIONAL_LIGHT  1
#define MAX_POINTLIGHT        32
#define MAX_SPOTLIGHT         32

namespace Elevate
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;
	class Shader;

	class SceneLighting
	{
	public:
		SceneLighting() = default;
		SceneLighting(DirectionalLight* dirLight, std::initializer_list<PointLight*> pointLights, std::initializer_list<SpotLight*> spotLights)
			: m_dirLight(dirLight), m_pointLights(pointLights), m_spotLights(spotLights) { }
		SceneLighting(DirectionalLight* dirLight, const std::vector<PointLight*>& pointLights, const std::vector<SpotLight*>& spotLights)
			: m_dirLight(dirLight), m_pointLights(pointLights), m_spotLights(spotLights) { }

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

	private:
		DirectionalLight* m_dirLight;
		std::vector<PointLight*> m_pointLights;
		std::vector<SpotLight*> m_spotLights;
	};
}