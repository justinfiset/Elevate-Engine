#include "Material.h"

#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>
#include <algorithm>
#include <string_view>

namespace Elevate
{
    MaterialID Material::s_nextId = EE_DEFAULT_MATERIAL + 1;

    Material::Material()
        : m_id(s_nextId++)
    {
    }

    Material::Material(const std::shared_ptr<Shader>& shader)
        : Material()
    {
        m_shader = shader;
        if (m_shader)
        {
            m_buffer.resize(m_shader->GetLayout().GetStride());
            m_definedUniforms.resize(m_shader->GetLayout().GetElements().size());
        }
    }

    void Material::SetTexture(const std::string& name, TexturePtr texture)
    {
        m_textures[name] = std::move(texture);
    }

    TexturePtr Material::GetTextureForUniform(const std::string& uniformName) const
    {
        if (m_textures.empty()) return nullptr;

        auto it = m_textures.find(uniformName);
        if (it != m_textures.end()) return it->second;

        static auto containsCaseless = [](std::string_view str, std::string_view sub) {
            return std::search(str.begin(), str.end(), sub.begin(), sub.end(),
                [](char a, char b) { return ::tolower(a) == ::tolower(b); }) != str.end();
            };

        static constexpr std::string_view keywords[] = { "diffuse", "specular", "ambient", "normal" };

        for (const auto& [key, tex] : m_textures)
        {
            for (auto kw : keywords)
            {
                if (containsCaseless(uniformName, kw) && containsCaseless(key, kw))
                    return tex;
            }
        }

        return nullptr;
    }

    void Material::Apply()
    {
        if (!m_shader) return;

        Renderer::BindShader(m_shader);

        const auto& layout = m_shader->GetLayout();

        for (const auto& uniform : layout)
        {
            if (uniform.Type == ShaderDataType::Sampler2D) continue;

            if (uniform.Name == EE_SHADER_VIEWPROJ ||
                uniform.Name == EE_SHADER_CAMPOS ||
                uniform.Name == EE_SHADER_MODEL)
            {
                continue;
            }

            if (m_definedUniforms[uniform.Index])
            {
                m_shader->SetUniform(uniform.Name, uniform.Type, m_buffer.data() + uniform.Offset);
            }
        }

        uint32_t slot = 0;
        std::string hasFlagName;
        hasFlagName.reserve(64);

        for (const auto& uniform : layout)
        {
            if (uniform.Type != ShaderDataType::Sampler2D) continue;

            const std::string& texName = uniform.Name;
            TexturePtr texture = GetTextureForUniform(texName);

            hasFlagName.assign("has_").append(texName);

            if (texture && texture->IsTextureLoaded() && texture->GetWidth() > 0 && texture->GetHeight() > 0)
            {
                Renderer::BindTexture(texture, slot);
                m_shader->SetUniform1i(texName, slot);
                m_shader->SetUniform1i(hasFlagName, 1);
            }
            else
            {
                Renderer::BindTexture(TextureManager::GetDefaultTexture(), slot);
                m_shader->SetUniform1i(texName, slot);
                m_shader->SetUniform1i(hasFlagName, 0);
            }

            slot++;
        }
    }

    std::shared_ptr<Shader> Material::GetShader() const
    {
        return m_shader;
    }

    MaterialPtr MaterialFactory::Create(const std::shared_ptr<Shader>& shader)
    {
        if (!shader) return nullptr;
        return std::shared_ptr<Material>(new Material(shader));
    }

    MaterialPtr MaterialRegistry::LoadMaterial(const std::shared_ptr<Shader>& shader)
    {
        if (!shader) return nullptr;
        MaterialPtr material = MaterialFactory::Create(shader);
        instance().m_materials[material->GetID()] = material;
        return material;
    }

    MaterialPtr MaterialRegistry::GetMaterial(MaterialID id)
    {
        auto& materials = instance().m_materials;
        auto it = materials.find(id);
        return (it != materials.end()) ? it->second : nullptr;
    }

    MaterialRegistry::MaterialRegistry()
    {
        m_materials[EE_DEFAULT_MATERIAL] = MaterialFactory::Create(ShaderManager::GetShader(EE_DEFAULT_SHADER));
    }

    MaterialRegistry& MaterialRegistry::instance()
    {
        static MaterialRegistry inst;
        return inst;
    }
}