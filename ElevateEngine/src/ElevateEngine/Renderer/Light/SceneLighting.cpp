#include "eepch.h"
#include <ElevateEngine/Renderer/Light/SceneLighting.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>

namespace Elevate
{
	void SceneLighting::UploadToShader(std::shared_ptr<Shader> shader) const
	{
		if (shader)
		{
			shader->UseDirLight(m_dirLight.get());
		}
	}
}
