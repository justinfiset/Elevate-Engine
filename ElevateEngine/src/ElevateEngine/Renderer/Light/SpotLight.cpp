#include "eepch.h"
#include "SpotLight.h"
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>
#include <ElevateEngine/Core/GameObject.h>


#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
#endif

namespace Elevate
{
	void SpotLight::Init()
	{
		gameObject->GetScene()->GetSceneLighting()->AddSpotLight(this);
	}

	void SpotLight::Destroy()
	{
		gameObject->GetScene()->GetSceneLighting()->RemoveSpotLight(this);
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
    void SpotLight::RenderWhenSelected()
    {
        const glm::vec4 rayColor = glm::vec4(1.0f, 0.85f, 0.1f, 1.0f);
		float range = m_attenuation.CalculateLightRange();
		float radius = std::tan(glm::radians(m_outerCone)) * range;
		Transform& transform = gameObject->GetTransform();
        DebugRenderer::AddDebugCone(transform.GetPosition(), transform.GetForward(), radius, range, 16, rayColor);
    }
#endif
}