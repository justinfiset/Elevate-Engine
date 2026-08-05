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
		SpotLight(float innerCone, float outerCone)
			: m_innerCone(innerCone), m_outerCone(outerCone) { }

		void UploadToShader(Shader* shader, uint32_t index) const;
		const glm::vec3 CalculateDirection() const;

#ifdef EE_EDITOR_BUILD
		virtual void RenderWhenSelected() override;
#endif

		float m_innerCone = 15.0f;
		PROPERTY(m_innerCone)
		float m_outerCone = 30.0f;
		PROPERTY(m_outerCone)

		LightAttenuation m_attenuation;
		PROPERTY(m_attenuation);

		ShadowSettings m_shadowSettings;
		PROPERTY(m_shadowSettings)

		END_COMPONENT()
	};
}