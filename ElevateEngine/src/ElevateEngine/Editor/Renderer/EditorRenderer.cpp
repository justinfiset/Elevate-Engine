#include "EditorRenderer.h"
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Mesh.h>
#include <ElevateEngine/Renderer/Material.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>

namespace Elevate
{
    void EditorRenderer::DrawBillboard(const glm::vec3& position, TexturePtr icon, float scale, glm::vec4 colorModifier)
    {
        if (!icon) return;

        static Mesh billboardQuad = Mesh::GenerateQuad();
        static ShaderPtr billboardShader = ShaderManager::GetShader("editor/billboard");

        if (!billboardShader)
        {
            EE_CORE_ERROR("Could not find billboard shader 'editor/billboard'.");
            return;
        }

        MaterialPtr instanceMaterial = MaterialRegistry::LoadMaterial(billboardShader);

        RenderState state;
        state.BlendMode = BlendModeType::Alpha;
        state.CullMode = CullFace::None;
        state.DepthTest = false;
        state.DepthWrite = false;

        instanceMaterial->SetRenderState(state);
        instanceMaterial->SetBucket(RenderBucket::Type::Editor);

        instanceMaterial->SetTexture("billboardTexture", icon);
        instanceMaterial->Set("colorModifier", colorModifier);
        instanceMaterial->Set("worldPos", position);
        instanceMaterial->Set("scale", scale);

        Renderer::SubmitMesh(billboardQuad.GetVertexArray(), instanceMaterial, glm::mat4(1.0f));
    }
}