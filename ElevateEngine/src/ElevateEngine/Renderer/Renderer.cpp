#include "eepch.h"
#include "Renderer.h"

#include "ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h"
#include <ElevateEngine/Scene/Scene.h>

#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Material.h>

namespace Elevate
{
    Renderer::RendererStorage Renderer::s_data = RendererStorage();
    RenderState Renderer::s_currentState = RenderState();
    RendererAPI* Renderer::s_API = new OpenGLRendererAPI();
    RenderCommandQueue Renderer::s_commands = RenderCommandQueue();
    uint32_t Renderer::s_currentShaderID = 0;
    uintptr_t Renderer::s_textures[16];
    static bool s_isStateCacheValid = false;

    void Renderer::BeginFrame(const ScenePtr scene, const Camera& cam)
    {
        InvalidateStateCache();

        s_API->ClearTextureBindings();

        s_data.CameraPosition = cam.gameObject->GetPosition();
        s_data.ViewProj = cam.GenViewProjectionMatrix();
        s_data.ActiveLighting = scene->GetSceneLighting();
    }

    bool Renderer::BindShader(const std::shared_ptr<Shader>& shader)
    {
        if (!shader) return false;

        uint32_t id = shader->GetID();
        if (s_currentShaderID != id)
        {
            shader->Bind();
            s_currentShaderID = id;
            return true;
        }
        return false;
    }

    void Renderer::ApplySystemUniforms(const std::shared_ptr<Shader>& shader)
    {
        // If the binded shader changed
        if (BindShader(shader))
        {
            shader->SetProjectionViewMatrix(s_data.ViewProj);
            shader->SetCameraPosition(s_data.CameraPosition);
        }
    }

    // RENDER API STATIC WRAPPER
    void Renderer::SetClearColor(const glm::vec4& color)
    {
        s_API->SetClearColor(color);
    }

    void Renderer::Clear()
    {
        s_API->Clear();
    }

    void Renderer::FlushBuffers()
    {
        s_API->FlushBuffers();
    }

    void Renderer::SetViewport(int x, int y, int width, int height)
    {
        s_API->SetViewport(x, y, width, height);
    }

    void Renderer::DrawArray(const VertexArray* vao, DrawPrimitiveType primitive)
    {
        if (vao)
        {
            s_API->DrawArray(vao, primitive);
        }
    }

    void Renderer::DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive)
    {
        DrawArray(vao.get(), primitive);
    }

    void Renderer::DrawStack()
    {
        s_commands.FlushAll();
    }

    void Renderer::PushRenderState(const RenderState& newState)
    {
        if (!s_isStateCacheValid || newState.Cullface != s_currentState.Cullface)
        {
            s_API->SetCullingState(newState.Cullface);
        }

        if (!s_isStateCacheValid || newState.DepthWrite != s_currentState.DepthWrite)
        {
            s_API->SetDepthWrittingState(newState.DepthWrite);
        }

        if (!s_isStateCacheValid || newState.DepthTest != s_currentState.DepthTest)
        {
            s_API->SetDepthTestingState(newState.DepthTest);
        }

        if (!s_isStateCacheValid || newState.BlendEnable != s_currentState.BlendEnable)
        {
            s_API->SetBlendingState(newState.BlendEnable);
        }

        s_currentState = newState;
        s_isStateCacheValid = true;
    }

    void Renderer::Dispatch(const RenderCommand& command)
    {
        PushRenderState(command.m_State);

        if (command.m_MaterialInstance)
        {
            auto shader = command.m_MaterialInstance->GetShader();
            if (shader)
            {
                ApplySystemUniforms(shader);
                shader->SetModelMatrix(command.Transform);
                if (s_data.ActiveLighting)
                {
                    s_data.ActiveLighting->UploadToShader(shader);
                }
                command.m_MaterialInstance->Apply();
            }
        }

        Renderer::DrawArray(command.m_VertexArray);
    }

    void Renderer::Submit(RenderBucket::Type type, const RenderCommand& command)
    {
        s_commands.Submit(type, command);
    }

    void Renderer::SubmitMesh(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& material, const glm::mat4& transform)
    {
        RenderCommand command;
        RenderState state = material ? material->GetRenderState() : RenderState();
        RenderBucket::Type bucket = material ? material->GetBucket() : RenderBucket::Opaque;

        command.m_VertexArray = vao.get();
        command.m_MaterialInstance = material.get();
        command.Transform = transform;
        command.m_State = state;

        Submit(bucket, command);
    }

    void Renderer::BindTexture(const std::shared_ptr<Texture>& texture, uint8_t slot)
    {
        if (slot >= 16) return;

        bool isLoaded = texture && texture->IsTextureLoaded() && texture->GetWidth() > 0;
        uintptr_t textureID = isLoaded ? reinterpret_cast<uintptr_t>(texture->GetNativeHandle()) : 0;

        if (s_textures[slot] != textureID)
        {
            if (isLoaded)
            {
                texture->Bind(slot);
            }
            else
            {
                s_API->UnbindTexture(slot);
            }
            s_textures[slot] = textureID;
        }
    }

    void Renderer::InvalidateStateCache()
    {
        for (size_t i = 0; i < std::size(s_textures); i++) {
            s_textures[i] = static_cast<uintptr_t>(-1);
        }
        s_currentShaderID = 0;
        s_isStateCacheValid = false;
    }
}