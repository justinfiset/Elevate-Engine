#include "eepch.h"

#include "SceneLighting.h"

#include <glm/gtc/matrix_transform.hpp>

#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Light/DirectionalLight.h>
#include <ElevateEngine/Renderer/Light/PointLight.h>
#include <ElevateEngine/Renderer/Light/SpotLight.h>

namespace Elevate
{
	void SceneLighting::UploadToShader(const std::shared_ptr<Shader>& shader) const
	{
		if (shader)
		{
			Renderer::BindShader(shader);

			if (m_dirLight)
			{
				shader->UseDirLight(m_dirLight);
			}

			size_t pointLightsCount = m_pointLights.size();
			shader->SetUniform1i("u_NumPointLights", (int)pointLightsCount);
			for (size_t i = 0; i < pointLightsCount; i++)
			{
				m_pointLights[i]->UploadToShader(shader.get(), (uint32_t)i);
			}

			size_t spotLightsCount = m_spotLights.size();
			shader->SetUniform1i("u_NumPointLights", (int)pointLightsCount);
			for (size_t i = 0; i < pointLightsCount; i++)
			{
				m_spotLights[i]->UploadToShader(shader.get(), (uint32_t)i);
			}
		}
	}

	glm::mat4 SceneLighting::GetDirectionalLightSpaceMatrix() const
	{
		if (!m_dirLight) return glm::mat4(1.0f);
		glm::mat4 modelMatrix = m_dirLight->gameObject->GetModelMatrix();
		glm::vec3 lightPos = glm::vec3(modelMatrix[3]);
		glm::vec3 lightDir = glm::normalize(-glm::vec3(modelMatrix[2]));
		glm::vec3 upDir = glm::normalize(glm::vec3(modelMatrix[1]));
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, upDir);

		// todo set un the light settings
		float shadowBoxSize = 20.0f;
		float nearPlane = 0.1f;
		float farPlane = 50.0f;

		return lightView * glm::ortho(
			-shadowBoxSize, shadowBoxSize,
			-shadowBoxSize, shadowBoxSize,
			nearPlane, farPlane
		);
	}
}
