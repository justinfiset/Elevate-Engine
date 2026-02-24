#include "eepch.h"
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Light/DirectionalLight.h>
#include <ElevateEngine/Renderer/Light/PointLight.h>

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
		}
	}
}
