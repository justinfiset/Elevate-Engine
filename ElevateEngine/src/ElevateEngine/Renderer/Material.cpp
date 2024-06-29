#include "eepch.h"
#include "Material.h"

const void Elevate::Material::Use(std::shared_ptr<Shader> shader) const
{
    shader->SetUniform3f("material.ambient", m_Ambient);
    shader->SetUniform3f("material.diffuse", m_Diffuse);
    shader->SetUniform3f("material.specular", m_Specular);
    shader->SetUniform1f("material.shininess", m_Shininess);
}
