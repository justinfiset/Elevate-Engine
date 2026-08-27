#include "eepch.h"

#include "SceneLighting.h"

#include <glm/gtc/matrix_transform.hpp>

#include <ElevateEngine/Renderer/Camera.h>
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

	glm::mat4 SceneLighting::GetDirectionalLightSpaceMatrix(const std::span<const glm::vec3> cameraCorners) const
	{
		if (!m_dirLight)
		{
			return glm::mat4(1.0f);
		}

		glm::mat4 lightWorld = m_dirLight->gameObject->GenGlobalMatrix();

		glm::vec3 center(0.0f);
		for (const auto& corner : cameraCorners)
		{
			center += corner;
		}
		center /= static_cast<float>(cameraCorners.size());

		glm::vec3 lightDir = glm::normalize(glm::vec3(lightWorld[2]));
		glm::vec3 upDir = glm::normalize(glm::vec3(lightWorld[1]));

		if (std::abs(glm::dot(lightDir, upDir)) > 0.99f)
		{
			upDir = glm::normalize(glm::vec3(lightWorld[0]));
		}

		glm::mat4 lightView = glm::lookAt(center + lightDir, center, upDir);
		glm::mat4 lightProjection = GetDirectionalLightProjection(cameraCorners, lightView);

		return lightProjection * lightView;
	}

	glm::mat4 SceneLighting::GetDirectionalLightProjection(const std::span<const glm::vec3> corners, const glm::mat4& lightView) const
	{
		const DirectionalShadowSettings& settings = m_dirLight->m_shadowSettings;

		glm::vec3 center(0.0f);
		for (const glm::vec3& corner : corners)
		{
			center += corner;
		}
		center /= static_cast<float>(corners.size());

		float radius = 0.0f;
		for (const glm::vec3& corner : corners)
		{
			radius = std::max(radius, glm::distance(center, corner));
		}

		glm::vec3 centerLightSpace = glm::vec3(lightView * glm::vec4(center, 1.0f));

		float shadowSize = radius * 2.0f;
		float texelSize = shadowSize / static_cast<float>(settings.Resolution);

		float minX = centerLightSpace.x - radius;
		float maxX = centerLightSpace.x + radius;
		float minY = centerLightSpace.y - radius;
		float maxY = centerLightSpace.y + radius;

		minX = std::floor(minX / texelSize) * texelSize;
		maxX = minX + shadowSize;

		minY = std::floor(minY / texelSize) * texelSize;
		maxY = minY + shadowSize;

		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();

		for (const glm::vec3& corner : corners)
		{
			glm::vec3 lightSpaceCorner = glm::vec3(lightView * glm::vec4(corner, 1.0f));
			minZ = std::min(minZ, lightSpaceCorner.z);
			maxZ = std::max(maxZ, lightSpaceCorner.z);
		}

		minZ -= settings.ZPadding;
		maxZ += settings.ZPadding;

		float nearPlane = -maxZ;
		float farPlane = -minZ;

		return glm::ortho(minX, maxX, minY, maxY, nearPlane, farPlane);
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
