#include "eepch.h"
#include "Renderer.h"

#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
#include <ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h>
#include <ElevateEngine/Scene/Scene.h>
#include <ElevateEngine/Renderer/Cubemap.h>

#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Material.h>
#include <ElevateEngine/Renderer/Framebuffer.h>

#include <ElevateEngine/Renderer/Light/Light.h>
#include <ElevateEngine/Renderer/Light/DirectionalLight.h>

constexpr uint32_t DEFAULT_SHADOW_RESOLUTION = 2048;

namespace Elevate
{
    Renderer::RendererStorage Renderer::s_data = RendererStorage();
    RenderState Renderer::s_currentState = RenderState();
    RendererAPI* Renderer::s_API = new OpenGLRendererAPI();
    RenderCommandQueue Renderer::s_commands = RenderCommandQueue();
    uint32_t Renderer::s_currentShaderID = 0;
    uintptr_t Renderer::s_textures[16];

    // Framebuffer
    std::unique_ptr<Framebuffer> Renderer::s_mainFramebuffer;

    // Shadows
    std::shared_ptr<Shader> Renderer::s_shadowShader;
    std::unique_ptr<Framebuffer> Renderer::s_directionalShadowMap;

    static bool s_isStateCacheValid = false;

    void Renderer::Init(uint32_t width, uint32_t height)
    {
        // Create the main color framebuffer
        s_mainFramebuffer.reset(Framebuffer::Create(width, height));
        s_mainFramebuffer->SetClearColor({ 0.8f, 0.4f, 0.7f, 1.0f }); // Pink / purple for debug purposes

        DebugRenderer::Init();
        InitShadowRenderer();
    }

    void Renderer::InitShadowRenderer()
    {
        // Create the shadow shader from files
        s_shadowShader = ShaderManager::LoadShader(
            "shadow",
            "engine://Shaders/Shadow.vert",
            "engine://Shaders/Shadow.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );

        // Create the Framebuffer
        s_directionalShadowMap.reset(Framebuffer::CreateDepthOnly(DEFAULT_SHADOW_RESOLUTION, DEFAULT_SHADOW_RESOLUTION));
    }

    void Renderer::BeginFrame(const ScenePtr scene, const Camera& cam)
    {
        InvalidateStateCache();

        s_API->ClearTextureBindings();

        s_data.CameraPosition = cam.gameObject->GetPosition();

        s_data.View = cam.GenViewMatrix();
        s_data.Projection = cam.GetProjectionMatrix();
        s_data.ViewProj = s_data.Projection * s_data.View;

        s_data.ActiveLighting = scene->GetSceneLighting();
        auto skybox = scene->GetSkybox().lock();
        s_data.LightSpaceMatrix = scene->GetSceneLighting()->GetDirectionalLightSpaceMatrix();
        s_data.ActiveCubemap = skybox.get();
    }

    void Renderer::RenderFrame()
    {
        RenderShaowMaps();
        RenderGeometry();

        s_mainFramebuffer->Bind();
        DebugRenderer::Render();
        s_mainFramebuffer->Unbind();

        ClearStack();
    }

    void Renderer::Present(uint32_t width, uint32_t height)
    {
        s_mainFramebuffer->BlitFramebufferToScreen(width, height);
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
            shader->SetUniformMatrix4fv("lightSpaceMatrix", s_data.LightSpaceMatrix);
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

    void Renderer::ClearDepth()
    {
        s_API->ClearDepth();
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

    void Renderer::ClearStack()
    {
        s_commands.Clear();
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

    Framebuffer& Renderer::GetMainFramebuffer()
    {
        return *s_mainFramebuffer;
    }

    Framebuffer& Renderer::GetDirectionalFrameBuffer()
    {
        return *s_directionalShadowMap;
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

                if (s_directionalShadowMap)
                {
                    BindTexture(s_directionalShadowMap->GetDepthTexture(), SHADOW_MAP_SLOT);
                    shader->SetUniform1i("shadowMap", SHADOW_MAP_SLOT);
                }
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
        RenderBucket::Type bucket = material ? material->GetBucket() : RenderBucket::GBuffer;

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

    void Renderer::RenderShaowMaps()
    {
        auto* dirLight = s_data.ActiveLighting->GetDirLight();
        if (dirLight)
        {
            glm::mat4 lightSpaceMatrix = s_data.ActiveLighting->GetDirectionalLightSpaceMatrix();
            DirectionalShadowSettings settings = dirLight->m_shadowSettings;
            
            BindShader(s_shadowShader);
            s_shadowShader->SetUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);

            // Rescale the framebuffer if the resolution was edited
            if (s_directionalShadowMap->GetWidth() != settings.Resolution || s_directionalShadowMap->GetHeight() != settings.Resolution)
            {
                s_directionalShadowMap->Rescale(settings.Resolution, settings.Resolution);
            }
            SetViewport(0, 0, settings.Resolution, settings.Resolution);

            s_directionalShadowMap->Bind();
            ClearDepth();
            
            RenderState shadowState;
            shadowState.Cullface = false; // todo : set to true
            shadowState.DepthTest = true;
            shadowState.DepthWrite = true;
            shadowState.BlendEnable = false;
            PushRenderState(shadowState);

            const auto& bucket = s_commands.GetBucket(RenderBucket::GBuffer);
            for (const auto& command : bucket)
            {
                s_shadowShader->SetModelMatrix(command.Transform);
                DrawArray(command.m_VertexArray);
            }

            s_directionalShadowMap->Unbind();
        }
    }

    void Renderer::RenderSkybox()
    {
        if (s_data.ActiveCubemap)
        {
            glm::mat4 view = glm::mat4(glm::mat3(s_data.View));
            s_data.ActiveCubemap->SetProjectionMatrix(s_data.Projection);
            s_data.ActiveCubemap->SetViewMatrix(view);
            s_data.ActiveCubemap->Draw();
        }
    }

    void Renderer::RenderGeometry()
    {
        s_mainFramebuffer->Bind();
        s_mainFramebuffer->Clear();
        SetViewport(0, 0, s_mainFramebuffer->GetWidth(), s_mainFramebuffer->GetHeight());
        RenderSkybox();
        DrawStack();

        s_mainFramebuffer->Unbind();
    }
}