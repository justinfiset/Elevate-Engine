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
			shader->SetUniform1i("u_NumSpotLights", (int)spotLightsCount);
			for (size_t i = 0; i < spotLightsCount; i++)
			{
				m_spotLights[i]->UploadToShader(shader.get(), (uint32_t)i);
			}
		}
	}

	const DirectionalLight* SceneLighting::GetDirLight() const
	{
		return m_dirLight;
	}

	glm::mat4 SceneLighting::GetDirectionalLightSpaceMatrix() const
	{
		if (!m_dirLight)
			return glm::mat4(1.0f);

		glm::mat4 lightWorld = m_dirLight->gameObject->GenGlobalMatrix();

		glm::vec3 lightPos = glm::vec3(lightWorld[3]);
		glm::vec3 lightDir = -glm::normalize(glm::vec3(lightWorld[2]));
		glm::vec3 upDir = glm::normalize(glm::vec3(lightWorld[1]));

		if (std::abs(glm::dot(lightDir, upDir)) > 0.99f)
		{
			upDir = glm::normalize(glm::vec3(lightWorld[0]));
		}

		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, upDir);

		const DirectionalShadowSettings& settings = m_dirLight->m_shadowSettings;
		float size = settings.OrthographicSize;

		glm::mat4 lightProjection = glm::ortho(
			-size, size,
			-size, size,
			settings.NearPlane,
			settings.FarPlane
		);

		return lightProjection * lightView;
	}

	void SceneLighting::AddDirectionalLight(DirectionalLight* light)
	{
		if (!light)
		{
			return;
		}
		m_dirLight = light;
	}

	void SceneLighting::AddPointLight(PointLight* light)
	{
		if (!light)
		{
			return;
		}
		
		if (std::find(m_pointLights.begin(), m_pointLights.end(), light) == m_pointLights.end())
		{
			m_pointLights.push_back(light);
		}
	}

	void SceneLighting::AddSpotLight(SpotLight* light)
	{
		if (!light)
		{
			return;
		}

		if (std::find(m_spotLights.begin(), m_spotLights.end(), light) == m_spotLights.end())
		{
			m_spotLights.push_back(light);
		}
	}

	void SceneLighting::RemoveDirectionalLight(DirectionalLight* light)
	{
		if (!light || m_dirLight != light)
		{
			return;
		}

		m_dirLight = nullptr;
	}

	void SceneLighting::RemovePointLight(PointLight* light)
	{
		if (!light)
		{
			return;
		}

		auto it = std::find(m_pointLights.begin(), m_pointLights.end(), light);
		if (it != m_pointLights.end())
		{
			m_pointLights.erase(it);
		}
	}

	void SceneLighting::RemoveSpotLight(SpotLight* light)
	{
		if (!light)
		{
			return;
		}

		auto it = std::find(m_spotLights.begin(), m_spotLights.end(), light);
		if (it != m_spotLights.end())
		{
			m_spotLights.erase(it);
		}
	}
}
