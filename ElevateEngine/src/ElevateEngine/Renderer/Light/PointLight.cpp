#include "eepch.h"
#include "PointLight.h"
#include "ElevateEngine/Renderer/Shader/Shader.h"
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate
{
    REGISTER_COMPONENT(PointLight);

    void PointLight::UploadToShader(Shader* shader, uint32_t index)
    {
        const std::string name = "pointLights[" + std::to_string(index) + "]";
        shader->UseLight(this, name);
        // Todo utiliser les paramettres
        shader->SetUniform3f(name + ".position", gameObject->GetGlobalPosition());
        shader->SetUniform1f(name + ".constant", 1.0f);
        shader->SetUniform1f(name + ".linear", 0.09f);
        shader->SetUniform1f(name + ".quadratic", 0.032f);
    }
}

