module;

#include <cstdint>


#include <glm/vec4.hpp>
#include <string>

module Elevate.Renderer.Lights.PointLight;

import Elevate.Core.Scene;
import Elevate.Renderer.Scene.Lighting;

#ifdef EE_EDITOR_BUILD
import Elevate.Editor.Renderer;
import Elevate.Editor.Selection;
#endif

import Elevate.Renderer.Shaders;

namespace Elevate
{
	void PointLight::Init()
	{
		if (auto* lighting = GetSceneLighting(gameObject->GetScene()))
		{
			lighting->AddPointLight(this);
		}
	}

	void PointLight::Destroy()
	{
		if (auto* lighting = GetSceneLighting(gameObject->GetScene()))
		{
			lighting->RemovePointLight(this);
		}
	}

	void PointLight::UploadToShader(Shader* shader, uint32_t index) const
	{
		const std::string name = "pointLights[" + std::to_string(index) + "]";
		shader->UseLight(this, name);
		shader->SetUniform3f(name + ".position", gameObject->GetGlobalPosition());
		shader->SetUniform1f(name + ".constant", m_attenuation.Constant);
		shader->SetUniform1f(name + ".linear", m_attenuation.Linear);
		shader->SetUniform1f(name + ".quadratic", m_attenuation.Quadratic);
	}

#ifdef EE_EDITOR_BUILD
	void PointLight::Render()
	{
		bool isSelected = Editor::Selection::IsSelected(gameObject);
		glm::vec4 colorMod = isSelected ? glm::vec4(1.5f, 1.5f, 0.5f, 1.0f)
			: glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		EditorRenderer::DrawBillboard(gameObject->GetGlobalPosition(), GetEditorIcon(), 0.5f, colorMod);
	}
#endif
}