#include "EditorRenderer.h"
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Mesh.h>
#include <ElevateEngine/Renderer/Material.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>

namespace Elevate
{
    void EditorRenderer::DrawBillboard(const glm::vec3& position, TexturePtr icon, float scale)
    {
        if (!icon) return;

        static Mesh billboardQuad = Mesh::GenerateQuad();
        static MaterialPtr billboardMaterial = nullptr;

        if (!billboardMaterial)
        {
            ShaderPtr shader = ShaderManager::GetShader("editor/billboard");
            if (!shader)
            {
                EE_CORE_ERROR("Could not find billboard shader 'editor/billboard'.");
                return;
            }

            billboardMaterial = MaterialRegistry::LoadMaterial(shader);

            RenderState state;
            state.BlendEnable = true;
            state.CullMode = CullFace::None;
            state.DepthTest = false;
            state.DepthWrite = false;
            
            billboardMaterial->SetRenderState(state);
            billboardMaterial->SetBucket(RenderBucket::Type::Transparent);
        }

        billboardMaterial->SetTexture("billboardTexture", icon);
        billboardMaterial->Set("worldPos", position);
        billboardMaterial->Set("scale", scale);

        Renderer::SubmitMesh(billboardQuad.GetVertexArray(), billboardMaterial, glm::mat4(1.0f));
    }
}