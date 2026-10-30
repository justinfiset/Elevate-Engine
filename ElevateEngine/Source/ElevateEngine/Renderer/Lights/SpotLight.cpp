module;

#include <string>
#include <glm/glm.hpp>

module Elevate.Renderer.Lights.SpotLight;

import Elevate.Core.Transform.Transform;
import Elevate.Renderer.Scene.Lighting;

#ifdef EE_EDITOR_BUILD
import Elevate.Renderer.Debug.DebugRenderer;
import Elevate.Editor.Renderer;
import Elevate.Editor.Selection;
#endif

namespace Elevate
{
	void SpotLight::Init()
	{
		GetSceneLighting(gameObject->GetScene())->AddSpotLight(this);
	}

	void SpotLight::Destroy()
	{
		GetSceneLighting(gameObject->GetScene())->RemoveSpotLight(this);
	}

	void SpotLight::UploadToShader(Shader* shader, uint32_t index) const
	{
		const std::string name = "spotLights[" + std::to_string(index) + "]";
		shader->UseLight(this, name);
		shader->SetUniform3f(name + ".position", gameObject->GetGlobalPosition());
		shader->SetUniform3f(name + ".direction", gameObject->GetTransform().GetForward());
		shader->SetUniform1f(name + ".constant", m_attenuation.Constant);
		shader->SetUniform1f(name + ".linear", m_attenuation.Linear);
		shader->SetUniform1f(name + ".quadratic", m_attenuation.Quadratic);
		shader->SetUniform1f(name + ".innerCutoff", glm::cos(glm::radians(m_innerCone)));
		shader->SetUniform1f(name + ".outerCutoff", glm::cos(glm::radians(m_outerCone)));
	}

#ifdef EE_EDITOR_BUILD
	void SpotLight::Render()
	{
		bool isSelected = Editor::Selection::IsSelected(gameObject);
		glm::vec4 colorMod = isSelected ? glm::vec4(1.5f, 1.5f, 0.5f, 1.0f)
			: glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		EditorRenderer::DrawBillboard(gameObject->GetGlobalPosition(), GetEditorIcon(), 0.5f);
	}

	void SpotLight::RenderWhenSelected()
	{
		const glm::vec4 rayColor = glm::vec4(1.0f, 0.85f, 0.1f, 1.0f);
		float range = m_attenuation.CalculateLightRange();
		float radius = std::tan(glm::radians(m_outerCone)) * range;
		Transform& transform = gameObject->GetTransform();
		DebugRenderer::AddDebugCone(gameObject->GetGlobalPosition(), transform.GetForward(), radius, range, 16, rayColor);
	}
#endif
}