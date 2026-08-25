#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Asset.h>
#include <ElevateEngine/Core/EEObjectPtr.h>
#include <ElevateEngine/Renderer/Buffer.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Commands/RenderBucket.h>

#define EE_DEFAULT_MATERIAL 0

namespace Elevate
{
    class Material;
    class MaterialFactory;
    class MaterialRegistry;

    using MaterialPtr = EEObjectPtr<Material>;
    using MaterialID = uint32_t;

    class Material : public Asset
    {
    public:
        virtual TypeLayout GetLayout() const override {
            std::vector<::Elevate::TypeField> allFields;

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
                    TypeField field(uniform.Name, uniform.Type, m_buffer.data() + uniform.Offset);
                    allFields.push_back(field);
                }
            }

            return TypeLayout(this, GetName(), allFields);
        }

        template<typename T>
        void Set(const std::string& name, const T& value)
        {
            for (const auto& uniform : m_shader->GetLayout())
            {
                if (uniform.Name == name)
                {
                    EE_ASSERT(uniform.Offset + sizeof(T) <= m_buffer.size(),
                        "Set Uniform: data size greater then buffer size!");

                    std::memcpy(m_buffer.data() + uniform.Offset, &value, sizeof(T));
                    m_definedUniforms[uniform.Index] = true;
                    return;
                }
            }
            EE_WARN("Uniform '{0}' not found in the shader : {1}", name, m_shader->GetID());
        }

        void SetTexture(const std::string& name, TexturePtr texture);
        void Apply();

        std::shared_ptr<Shader> GetShader() const;
        MaterialID GetID() const { return m_id; }

        RenderState& GetRenderState() { return m_state; }
        const RenderState& GetRenderState() const { return m_state; }
        void SetRenderState(const RenderState& state) { m_state = state; }

        RenderBucket::Type GetBucket() const { return m_bucket; }
        void SetBucket(RenderBucket::Type bucket) { m_bucket = bucket; }

        size_t GetTextureCount() const { return m_textures.size(); }

    private:
        Material();
        Material(const std::shared_ptr<Shader>& shader);

        TexturePtr GetTextureForUniform(const std::string& uniformName) const;

        std::shared_ptr<Shader> m_shader{ nullptr };

        std::vector<uint8_t> m_buffer;
        std::vector<bool> m_definedUniforms;

        std::unordered_map<std::string, TexturePtr> m_textures;

        MaterialID m_id{ 0 };
        static MaterialID s_nextId;

        RenderState m_state;
        RenderBucket::Type m_bucket = RenderBucket::GBuffer;

        friend class MaterialFactory;
    };

    class MaterialFactory
    {
    protected:
        static MaterialPtr Create(const std::shared_ptr<Shader>& shader);
        friend class MaterialRegistry;
    };

    class MaterialRegistry
    {
    public:
        static MaterialPtr LoadMaterial(const std::shared_ptr<Shader>& shader);
        static MaterialPtr GetMaterial(MaterialID id);

    private:
        MaterialRegistry();
        static MaterialRegistry& instance();

        std::unordered_map<MaterialID, MaterialPtr> m_materials;
    };
}