#include "eepch.h"
#include "SpotLight.h"
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate
{
	const glm::vec3 SpotLight::CalculateDirection() const
	{
		glm::vec3 dir;

		glm::vec3 rotation = gameObject->GetRotation();
		dir.x = cos(rotation.x) * cos(rotation.y);
		dir.y = sin(rotation.x);
		dir.z = cos(rotation.x) * sin(rotation.y);
		dir = glm::normalize(dir);

		return dir;
	}

	void SpotLight::UploadToShader(Shader* shader, uint32_t index) const
	{
		 const std::string name = "spotLights[" + std::to_string(index) + "]";
		 shader->UseLight(this, name);
		 // Todo utiliser les paramettres
		 shader->SetUniform3f(name + ".position", gameObject->GetGlobalPosition());
		 shader->SetUniform3f(name + ".direction", CalculateDirection());
		 shader->SetUniform1f(name + ".constant", 1.0f);
		 shader->SetUniform1f(name + ".linear", 0.09f);
		 shader->SetUniform1f(name + ".quadratic", 0.032f);
		 shader->SetUniform1f(name + ".innerCutoff", glm::cos(glm::radians(m_innerCone)));
		 shader->SetUniform1f(name + ".outerCutoff", glm::cos(glm::radians(m_outerCone)));
	}
}