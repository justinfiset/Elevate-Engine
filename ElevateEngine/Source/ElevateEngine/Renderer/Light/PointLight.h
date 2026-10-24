#pragma once
#include "Light.h"

#include "Reflection.h"

import Elevate.Core.Scene;

namespace Elevate
{
	class PointLight : public Light
	{
		BEGIN_COMPONENT(PointLight, EditorIconTag(Editor::Icons::COMPONENT_ICON_POINTLIGHT));
		DECLARE_BASE(Light);
		EECATEGORY("Lighting");
	public:
		PointLight() = default;
		PointLight(const glm::vec3& color) : Light(color) {}

		virtual void Init() override;
		virtual void Destroy() override;

		void UploadToShader(Shader* shader, uint32_t index) const;

#ifdef EE_EDITOR_BUILD
		virtual void Render() override;
#endif

		LightAttenuation m_attenuation;
		PROPERTY(m_attenuation);

		ShadowSettings m_shadowSettings;
		PROPERTY(m_shadowSettings);

		END_COMPONENT();
	};
}