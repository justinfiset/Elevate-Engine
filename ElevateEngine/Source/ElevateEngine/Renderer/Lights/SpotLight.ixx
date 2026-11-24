module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <glm/glm.hpp>
#include "Reflection.h"

export module Elevate.Renderer.Lights.SpotLight;

import Elevate.Core.Scene;
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Lights.Light;
import Elevate.Renderer.Lights.Contract;

export namespace Elevate
{
	class SpotLight : public Light, public ISpotLight
	{
		BEGIN_COMPONENT(SpotLight, EditorIconTag(Editor::Resources::Icons::Light::Flare));
		DECLARE_BASE(Light);
		EECATEGORY("Lighting");

	public:
		SpotLight() = default;
		SpotLight(float innerCone, float outerCone)
			: m_innerCone(innerCone), m_outerCone(outerCone)
		{
		}

		void Init() override;
		void Destroy() override;

		void UploadToShader(Shader* shader, uint32_t index) const override;
		const glm::vec3 CalculateDirection() const;

#ifdef EE_EDITOR_BUILD
		void Render() override;
		void RenderWhenSelected() override;
#endif

		float m_innerCone = 15.0f;
		PROPERTY(m_innerCone);
		float m_outerCone = 30.0f;
		PROPERTY(m_outerCone);

		LightAttenuation m_attenuation;
		PROPERTY(m_attenuation);

		ShadowSettings m_shadowSettings;
		PROPERTY(m_shadowSettings);

		END_COMPONENT();
	};
}