#pragma once
#include "Light.h"

namespace Elevate
{
	class PointLight : public Light
	{
		BEGIN_COMPONENT(PointLight)
		DECLARE_BASE(Light)
		EECATEGORY("Lighting")
	public:
		PointLight() = default;
		PointLight(const glm::vec3& color) : Light(color) { }
		
		virtual void Init() override;
		virtual void Destroy() override;

		void UploadToShader(Shader* shader, uint32_t index) const;

		LightAttenuation m_attenuation;
		PROPERTY(m_attenuation);

		ShadowSettings m_shadowSettings;
		PROPERTY(m_shadowSettings)

		END_COMPONENT()
	};
}