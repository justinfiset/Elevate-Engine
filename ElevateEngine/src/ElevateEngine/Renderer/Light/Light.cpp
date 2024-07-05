#include "eepch.h"
#include "Light.h"

const void Elevate::Light::Use(std::shared_ptr<Shader> shader)
{
    shader->SetUniform3f("dirLight.ambient", m_ambientColor);
    shader->SetUniform3f("dirLight.diffuse", m_diffuseColor);
    shader->SetUniform3f("dirLight.specular", m_specularColor);
}
