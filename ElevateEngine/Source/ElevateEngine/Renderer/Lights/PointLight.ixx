module;

#include <glm/glm.hpp>
#include "Reflection.h"

export module Elevate.Renderer.Lights.PointLight;

// Light already imports Elevate.Core.Scene.  Importing it here as well makes
// GCC 15 load the Scene BMI twice and duplicate Component's declarations.
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Lights.Light;

export namespace Elevate
{
	class PointLight : public Light
	{
		BEGIN_COMPONENT(PointLight, EditorIconTag(Editor::Resources::Icons::Light::Lightbulb));
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