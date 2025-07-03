#pragma once

#include <memory>

#include "ElevateEngine/Renderer/Light/DirectionalLight.h"

namespace Elevate
{
	class SceneLighting
	{
	public:
		void UploadToShader(std::shared_ptr<Shader> shader) const;
	private:
		std::unique_ptr<DirectionalLight> m_dirLight;
		std::vector<std::shared_ptr<Light
	};
}