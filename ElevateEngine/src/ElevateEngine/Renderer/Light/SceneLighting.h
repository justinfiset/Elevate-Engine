#pragma once

#include <memory>

// The maximum number of each lights to have in the game, these numbers are passed to the shader at compile time
#define MAX_DIRECTIONAL_LIGHT  1
#define MAX_POINTLIGHT        32
#define MAX_SPOTLIGHT         32

namespace Elevate
{
	class DirectionalLight;
	class PointLight;
	class Shader;

	class SceneLighting
	{
	public:
		SceneLighting() = default;
		SceneLighting(DirectionalLight* dirLight, std::initializer_list<PointLight*> pointLights)
			: m_dirLight(dirLight), m_pointLights(pointLights) { }
		SceneLighting(DirectionalLight* dirLight, const std::vector<PointLight*>& pointLights)
			: m_dirLight(dirLight), m_pointLights(pointLights) {}

		void UploadToShader(Shader* shader) const;
	private:
		DirectionalLight* m_dirLight;
		std::vector<PointLight*> m_pointLights;
	};
}