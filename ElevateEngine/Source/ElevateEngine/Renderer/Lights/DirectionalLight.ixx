module;


#include <glm/glm.hpp>
#include "Reflection.h"

export module Elevate.Renderer.Lights.DirectionalLight;

import Elevate.Core.Scene;
import Elevate.Renderer.Lights.Light;
import Elevate.Renderer.Lights.Contract;

export namespace Elevate
{
	class DirectionalLight : public Light, public IDirectionalLight
	{
	public:
		BEGIN_COMPONENT(DirectionalLight, EditorIconTag(Editor::Resources::Icons::Light::Sun));
		DECLARE_BASE(Light);
		EECATEGORY("Lighting");

		DirectionalLight() = default;
		DirectionalLight(const glm::vec3& color) : Light(color) {}

		void Init() override;
		void Destroy() override;

		const Light& GetLight() const override { return *this; }
		glm::vec3 CalculateDirection() const override;
		const DirectionalShadowSettings& GetShadowSettings() const override { return m_shadowSettings; }
		glm::mat4 GetWorldMatrix() const override;

#ifdef EE_EDITOR_BUILD
		void Render() override;
		void RenderWhenSelected() override;
		void DrawDebugDirection();
#endif

		DirectionalShadowSettings m_shadowSettings;
		PROPERTY(m_shadowSettings);

		END_COMPONENT();
	};
}