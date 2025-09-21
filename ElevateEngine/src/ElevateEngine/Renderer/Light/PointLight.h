#pragma once
#include "Light.h"

namespace Elevate
{
	class PointLight : public Light
	{
		BEGIN_COMPONENT(PointLight)
		DECLARE_BASE(Light)
	public:
		PointLight() = default;
		PointLight(const glm::vec3& color) : Light(color) { }
		void UploadToShader(Shader* shader, uint32_t index);
		
		END_COMPONENT()
	};
}