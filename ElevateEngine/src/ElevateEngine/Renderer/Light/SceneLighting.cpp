#include "eepch.h"
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Light/DirectionalLight.h>
#include <ElevateEngine/Renderer/Light/PointLight.h>

namespace Elevate
{
	void SceneLighting::UploadToShader(Shader* shader) const
	{
		if (shader)
		{
			shader->Bind();

			if (m_dirLight)
			{
				shader->UseDirLight(m_dirLight);
			}

			for (size_t i = 0; i < m_pointLights.size(); i++)
			{
				m_pointLights[i]->UploadToShader(shader, (uint32_t) i);
			}
		}
	}
}
