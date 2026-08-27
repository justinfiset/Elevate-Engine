#include "eepch.h"
#include "PointLight.h"
#include "ElevateEngine/Renderer/Shader/Shader.h"
#include <ElevateEngine/Renderer/Light/SceneLighting.h>
#include <ElevateEngine/Core/GameObject.h>

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Editor/Renderer/EditorRenderer.h>
#endif

namespace Elevate
{
	void PointLight::Init()
	{
		gameObject->GetScene()->GetSceneLighting()->AddPointLight(this);
	}

	void PointLight::Destroy()
	{
		gameObject->GetScene()->GetSceneLighting()->RemovePointLight(this);
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
		EditorRenderer::DrawBillboard(gameObject->GetGlobalPosition(), GetEditorIcon(), 0.5f);
	}
#endif
}