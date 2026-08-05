#pragma once
#include "Light.h"

namespace Elevate
{	
	class DirectionalLight : public Light
	{
	public:
		BEGIN_COMPONENT(DirectionalLight)
		DECLARE_BASE(Light)
		EECATEGORY("Lighting")

		DirectionalLight() = default;
		DirectionalLight(const glm::vec3& color) : Light(color) { }

		const glm::vec3 CalculateDirection() const;
		
#ifdef EE_EDITOR_BUILD
		void RenderWhenSelected();
		void DrawDebugDirection();
#endif

		DirectionalShadowSettings m_shadowSettings;
		PROPERTY(m_shadowSettings)

		END_COMPONENT()
	};
}