#pragma once
#include "Light.h"

namespace Elevate
{
	class SpotLight : public Light
	{
		BEGIN_COMPONENT(SpotLight)
		DECLARE_BASE(Light)
		EECATEGORY("Lighting")
	public:
		SpotLight() = default;
		SpotLight(const glm::vec3& color) : Light(color) { }
		void UploadToShader(Shader* shader, uint32_t index);
		
		float m_range = 10.0f;
		PROPERTY(m_range)
		float m_innerCone = 15.0f;
		PROPERTY(m_innerCone)
		float m_outerCone = 30.0f;
		PROPERTY(m_outerCone)

		END_COMPONENT()
	};
}