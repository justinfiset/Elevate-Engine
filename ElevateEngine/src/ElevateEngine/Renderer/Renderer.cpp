#include "eepch.h"
#include "Renderer.h"

#include <random>

#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
#include <ElevateEngine/Renderer/Mesh.h>
#include <ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h>
#include <ElevateEngine/Scene/Scene.h>
#include <ElevateEngine/Renderer/Cubemap.h>

#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Material.h>
#include <ElevateEngine/Renderer/Framebuffer.h>

#include <ElevateEngine/Renderer/Light/Light.h>
#include <ElevateEngine/Renderer/Light/DirectionalLight.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

constexpr uint32_t DEFAULT_SHADOW_RESOLUTION = 2048;
constexpr uint16_t AO_KERNER_SAMPLE_COUNT = 64;

namespace Elevate
{
    RendererStorage Renderer::s_data = RendererStorage();
    RenderState Renderer::s_currentState = RenderState();
    std::unique_ptr<RendererAPI> Renderer::s_API;
    RenderCommandQueue Renderer::s_commands = RenderCommandQueue();
    uint32_t Renderer::s_currentShaderID = 0;
    uintptr_t Renderer::s_textures[16];
    Mesh Renderer::s_fullscreenQuad;

    // Framebuffer
    std::unique_ptr<Framebuffer> Renderer::s_geometryFramebuffer;

    // Shadows
    std::shared_ptr<Shader> Renderer::s_shadowShader;
    std::array<std::unique_ptr<Framebuffer>, SHADOW_CASCADE_COUNT> Renderer::s_directionalShadowMaps;

    // Lighting
    std::shared_ptr<Shader> Renderer::s_lightPassShader;
    std::unique_ptr<Framebuffer> Renderer::s_lightPassFramebuffer;

    // SSAO
    std::vector<glm::vec3> Renderer::s_ssaoKernel;
    std::shared_ptr<Shader> Renderer::s_ssaoShader;
    std::unique_ptr<Elevate::Framebuffer> Renderer::s_ssaoFramebuffer;
    std::shared_ptr<Shader> Renderer::s_ssaoBlurShader;
    std::unique_ptr<Framebuffer> Renderer::s_ssaoBlurFramebuffer;
    TexturePtr Renderer::s_ssaoNoiseTexture;

    // Bloom
    std::vector<std::unique_ptr<Framebuffer>> Renderer::s_bloomMipChain;
    std::shared_ptr<Shader> Renderer::s_bloomDownsampleShader;
    std::shared_ptr<Shader> Renderer::s_bloomUpsampleShader;

    // Composition
    std::unique_ptr<Framebuffer> Renderer::s_mainFramebuffer;
    std::shared_ptr<Shader> Renderer::s_compositionShader;

    static bool s_isStateCacheValid = false;

    void Renderer::Init(uint32_t width, uint32_t height)
    {
        s_API = std::make_unique<OpenGLRendererAPI>();

        // Create the quad to render to the screen
        s_fullscreenQuad = Mesh::GenerateQuad(2.0f);

        // Create the gbuffer
        s_geometryFramebuffer.reset(Framebuffer::Create
        (
            width,
            height,
            {
                TextureFormat::RGBA16F,    // Color : Albedo (RGBA)
                TextureFormat::RGB16F,  // Normals (RGB)
                TextureFormat::RGBA     // Material : Roughness (R) + Metallic (G) + AO (B) + Extra (A)
            },
            false
        ));

        s_geometryFramebuffer->SetClearColor({ 0.8f, 0.4f, 0.7f, 1.0f }); // Pink / purple for debug purposes
        s_mainFramebuffer.reset(Framebuffer::Create(width, height, { TextureFormat::RGB }));
        s_mainFramebuffer->SetClearColor({ 0.8f, 0.4f, 0.7f, 1.0f }); // Pink / purple for debug purposes

        s_compositionShader = ShaderManager::LoadShader(
            "composition",
            "engine://Shaders/Composition.vert",
            "engine://Shaders/Composition.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );

        DebugRenderer::Init();
        InitShadowRenderer();
        InitLightingRenderer(width, height);
        InitSSAORenderer(width, height);
        InitBloomRenderer(width, height);
    }

    static float RandomFloat(float min, float max)
    {
        static std::random_device rd;
        static std::mt19937 generator(12345);
        std::uniform_real_distribution<float> distribution(min, max);

        return distribution(generator);
    }

    void Renderer::InitSSAORenderer(uint32_t width, uint32_t height)
    {
        s_ssaoShader = ShaderManager::LoadShader(
            "ssao",
            "engine://Shaders/SSAO.vert",
            "engine://Shaders/SSAO.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );

        s_ssaoBlurShader = ShaderManager::LoadShader(
            "ssaoBlur",
            "engine://Shaders/SSAOBlur.vert",
            "engine://Shaders/SSAOBlur.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );

        std::vector<glm::vec3> ssaoNoise;
        std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
        std::default_random_engine generator;

        for (uint32_t i = 0; i < 16; ++i)
        {
            glm::vec3 noise(
                randomFloats(generator) * 2.0f - 1.0f,
                randomFloats(generator) * 2.0f - 1.0f,
                0.0f
            );
            ssaoNoise.push_back(glm::normalize(noise));
        }

        TextureMetadata noiseMeta = TextureMetadataBuilder()
            .Name("AONoiseTexture")
            .size(4, 4)
            .Format(TextureFormat::RGB32F)
            .Usage(TextureType::Diffuse)
            .Source(TextureSource::Generated)
            .State(TextureState::Ready)
            .Filter(TextureFilter::Nearest, TextureFilter::Nearest)
            .Wrap(TextureWrap::Repeat, TextureWrap::Repeat)
            .Mipmaps(false)
            .Build();

        s_ssaoNoiseTexture = Texture::CreateFromData(ssaoNoise.data(), noiseMeta);

        s_ssaoKernel.reserve(AO_KERNER_SAMPLE_COUNT);
        for (int i = 0; i < AO_KERNER_SAMPLE_COUNT; i++)
        {
            glm::vec3 sample(
                RandomFloat(-1.0f, 1.0f),
                RandomFloat(-1.0f, 1.0f),
                RandomFloat(0.0f, 1.0f)
            );

            float scale = static_cast<float>(i) / static_cast<float>(AO_KERNER_SAMPLE_COUNT);

            scale = glm::mix(
                0.1f,
                1.0f,
                scale * scale
            );

            sample *= scale;

            s_ssaoKernel.push_back(sample);
        }

        // Create the Framebuffer   
        s_ssaoFramebuffer.reset(Framebuffer::Create(width, height, { TextureFormat::RGBA })); // todo R8 (red)
        s_ssaoBlurFramebuffer.reset(Framebuffer::Create(width, height, { TextureFormat::RGBA })); // todo R8 (red)
    }

    void Renderer::InitBloomRenderer(uint32_t width, uint32_t height)
    {
        s_bloomMipChain.clear();

        s_bloomDownsampleShader = ShaderManager::LoadShader(
            "BloomDownscale",
            "engine://Shaders/BloomDownscale.vert",
            "engine://Shaders/BloomDownscale.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );
        s_bloomUpsampleShader = ShaderManager::LoadShader(
            "BloomUpscale",
            "engine://Shaders/BloomUpscale.vert",
            "engine://Shaders/BloomUpscale.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );

        uint32_t currentWidth = width / 2;
        uint32_t currentHeight = height / 2;
        
        const uint8_t maxMips = 6;
        for (uint8_t i = 0; i < maxMips; i++)
        {
            if (currentWidth < 1 || currentHeight < 1)
            {
                break;
            }

            auto fb = Framebuffer::Create(currentWidth, currentHeight, { TextureFormat::RGBA16F });
            s_bloomMipChain.push_back(std::unique_ptr<Framebuffer>(fb));

            currentWidth /= 2;
            currentHeight /= 2;
        }
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

        // Create the Framebuffers
        for (int i = 0; i < SHADOW_CASCADE_COUNT; i++)
        {
            s_directionalShadowMaps[i].reset(Framebuffer::CreateDepthOnly(DEFAULT_SHADOW_RESOLUTION, DEFAULT_SHADOW_RESOLUTION));
        }
    }

    void Renderer::InitLightingRenderer(uint32_t width, uint32_t height)
    {
        s_lightPassFramebuffer.reset(Framebuffer::Create(width, height, { TextureFormat::RGBA16F }));
        s_lightPassShader = ShaderManager::LoadShader(
            "lighting",
            "engine://Shaders/Lighting.vert",
            "engine://Shaders/Lighting.frag",
            EE_SHADER_HEADER,
            EE_SHADER_HEADER
        );
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

        constexpr std::array<float, SHADOW_CASCADE_COUNT + 1> cascadeSplits = { 0.0f, 0.05f, 0.15f, 0.40f, 1.0f };
        float clipRange = cam.GetFar() - cam.GetNear();

        for (int i = 0; i < SHADOW_CASCADE_COUNT; i++)
        {
            float currentNear = cascadeSplits[i];
            float currentFar = cascadeSplits[i + 1];

            auto corners = cam.CalculateFrustumCorners(currentNear, currentFar);
            s_data.LightSpaceMatrices[i] = scene->GetSceneLighting()->GetDirectionalLightSpaceMatrix(corners);
            s_data.CascadeSplitDepths[i] = cam.GetNear() + (currentFar * clipRange);
        }

        s_data.ActiveCubemap = skybox.get();
    }

    void Renderer::RenderFrame()
    {
        RenderShaowMaps();
        RenderGeometry();
        RenderSSAO();
        RenderLighting();
        RenderBloom();
        RenderComposition();

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
        BindShader(shader);

        shader->SetProjectionViewMatrix(s_data.ViewProj);
        shader->SetCameraPosition(s_data.CameraPosition);
        shader->SetUniformMatrix4fv("view", s_data.View);

        shader->SetUniformMatrix4fv("inverseProjection", glm::inverse(s_data.Projection));
        shader->SetUniformMatrix4fv("inverseView", glm::inverse(s_data.View));

        shader->SetUniformMatrix4fv(
            "lightSpaceMatrices[0]",
            reinterpret_cast<float*>(s_data.LightSpaceMatrices.data()),
            SHADOW_CASCADE_COUNT
        );

        shader->SetUniform1fv(
            "cascadeSplitDepths[0]",
            SHADOW_CASCADE_COUNT,
            s_data.CascadeSplitDepths.data()
        );
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
        if (!s_isStateCacheValid || newState.CullMode != s_currentState.CullMode)
        {
            s_API->SetCullingState(newState.CullMode);
        }

        if (!s_isStateCacheValid || newState.DepthWrite != s_currentState.DepthWrite)
        {
            s_API->SetDepthWrittingState(newState.DepthWrite);
        }

        if (!s_isStateCacheValid || newState.DepthTest != s_currentState.DepthTest)
        {
            s_API->SetDepthTestingState(newState.DepthTest);
        }

        if (!s_isStateCacheValid || newState.BlendMode != s_currentState.BlendMode)
        {
            s_API->SetBlendingState(newState.BlendMode);
        }

        s_currentState = newState;
        s_isStateCacheValid = true;
    }

    const RendererStorage& Renderer::GetFrameData()
    {
        return s_data;
    }

    Framebuffer& Renderer::GetMainFramebuffer()
    {
        return *s_mainFramebuffer;
    }   

    Framebuffer& Renderer::GetDirectionalFrameBuffer(uint32_t index)
    {
        return *s_directionalShadowMaps[index]; // todo correct this
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
                
                command.m_MaterialInstance->Apply();
            }
        }

        Renderer::DrawArray(command.m_VertexArray);
    }

    void Renderer::Submit(RenderBucket::Type type, const RenderCommand& command)
    {
        s_commands.Submit(type, command);
    }

    void Renderer::SubmitMesh(const std::shared_ptr<VertexArray>& vao, const MaterialPtr& material, const glm::mat4& transform)
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
            DirectionalShadowSettings settings = dirLight->m_shadowSettings;
            
            BindShader(s_shadowShader);

            // Rescale the framebuffer if the resolution was edited
            if (s_directionalShadowMaps[0]->GetWidth() != settings.Resolution || s_directionalShadowMaps[0]->GetHeight() != settings.Resolution)
            {
                for (int i = 0; i < SHADOW_CASCADE_COUNT; i++)
                {
                    s_directionalShadowMaps[i]->Rescale(settings.Resolution, settings.Resolution);
                }
            }

            RenderState shadowState;
            shadowState.CullMode = CullFace::Front;
            shadowState.DepthTest = true;
            shadowState.DepthWrite = true;
            shadowState.BlendMode = BlendMode::None;
            PushRenderState(shadowState);

            const auto& bucket = s_commands.GetBucket(RenderBucket::GBuffer);

            for (int i = 0; i < SHADOW_CASCADE_COUNT; i++)
            {
                s_directionalShadowMaps[i]->Use();
                ClearDepth();

                s_shadowShader->SetUniformMatrix4fv("lightSpaceMatrix", s_data.LightSpaceMatrices[i]);

                for (const auto& command : bucket)
                {
                    s_shadowShader->SetModelMatrix(command.Transform);
                    DrawArray(command.m_VertexArray);
                }

                s_directionalShadowMaps[i]->Unbind();
            }
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
        s_geometryFramebuffer->ClearAndUse();

        RenderSkybox();
        DrawStack();

        s_geometryFramebuffer->Unbind();
    }

    void Renderer::RenderLighting()
    {
        RenderState lightingState;
        lightingState.BlendMode = BlendMode::None;
        lightingState.CullMode = CullFace::None;
        lightingState.DepthTest = false;
        lightingState.DepthWrite = false;
        PushRenderState(lightingState);

        s_lightPassFramebuffer->ClearAndUse();

        BindShader(s_lightPassShader);
        
        ApplySystemUniforms(s_lightPassShader);

        // Lighting configuration
        if (s_data.ActiveLighting)
        {
            s_data.ActiveLighting->UploadToShader(s_lightPassShader);
        }

        // Shadows
        if (s_directionalShadowMaps[0])
        {
            int shadowSlots[SHADOW_CASCADE_COUNT];
            for (int i = 0; i < SHADOW_CASCADE_COUNT; i++)
            {
                uint8_t slot = SHADOW_MAP_SLOT + i;
                BindTexture(s_directionalShadowMaps[i]->GetDepthTexture(), slot);
                shadowSlots[i] = slot;
            }

            s_lightPassShader->SetUniform1iv("shadowMapArray", SHADOW_CASCADE_COUNT, shadowSlots);
        }

        // GBuffer
        BindTexture(s_geometryFramebuffer->GetColorTexture(0), 0);
        s_lightPassShader->SetUniform1i("g_Color", 0);
        BindTexture(s_geometryFramebuffer->GetColorTexture(1), 1);
        s_lightPassShader->SetUniform1i("g_Normal", 1);
        BindTexture(s_geometryFramebuffer->GetColorTexture(2), 2);
        s_lightPassShader->SetUniform1i("g_Material", 2);
        BindTexture(s_geometryFramebuffer->GetDepthTexture(), 3);
        s_lightPassShader->SetUniform1i("g_Depth", 3);
        // SSAO
        BindTexture(s_ssaoBlurFramebuffer->GetColorTexture(), 4);
        s_lightPassShader->SetUniform1i("g_SSAO", 4);

        DrawArray(s_fullscreenQuad.GetVertexArray());
        s_lightPassFramebuffer->Unbind();
    }

    void Renderer::RenderSSAO()
    {
        s_ssaoFramebuffer->ClearAndUse();

        BindShader(s_ssaoShader);

        // Bind the gDepth
        BindTexture(s_geometryFramebuffer->GetDepthTexture(), 0);
        s_ssaoShader->SetUniform1i("gDepth", 0);
        // Bind the gNormal
        BindTexture(s_geometryFramebuffer->GetColorTexture(1), 1);
        s_ssaoShader->SetUniform1i("gNormal", 1);
        // Bind the noise texture
        BindTexture(s_ssaoNoiseTexture, 2);
        s_ssaoShader->SetUniform1i("noiseTexture", 2);

        glm::vec2 noiseScale(
            static_cast<float>(s_ssaoFramebuffer->GetWidth()) / 4.0f,
            static_cast<float>(s_ssaoFramebuffer->GetHeight()) / 4.0f
        );
        s_ssaoShader->SetUniform2f("noiseScale", noiseScale);


        s_ssaoShader->SetUniformMatrix4fv("inverseProjection", glm::inverse(s_data.Projection));
        s_ssaoShader->SetUniformMatrix4fv("projection", s_data.Projection);
        s_ssaoShader->SetUniformMatrix4fv("view", s_data.View);

        for (int i = 0; i < s_ssaoKernel.size(); i++)
        {
            s_ssaoShader->SetUniform3f("samples[" + std::to_string(i) + "]", s_ssaoKernel[i]);
        }

        DrawArray(s_fullscreenQuad.GetVertexArray());
        s_ssaoFramebuffer->Unbind();


        // Second pass to blur
        s_ssaoBlurFramebuffer->ClearAndUse();

        BindShader(s_ssaoBlurShader);
        BindTexture(s_ssaoFramebuffer->GetColorTexture(), 0);
        s_ssaoBlurShader->SetUniform1i("aoTexture", 0);
        BindTexture(s_geometryFramebuffer->GetDepthTexture(), 1);
        s_ssaoBlurShader->SetUniform1i("gDepth", 1);


        DrawArray(s_fullscreenQuad.GetVertexArray());
        s_ssaoBlurFramebuffer->Unbind();
    }

    void Renderer::RenderBloom()
    {
        RenderState state;

        state.BlendMode = BlendMode::None;
        state.CullMode = CullFace::None;
        state.DepthTest = false;
        state.DepthWrite = false;
        PushRenderState(state);

        BindShader(s_bloomDownsampleShader);

        TexturePtr currentSource = s_lightPassFramebuffer->GetColorTexture();

        // Downsample trought the bloom mip chain
        for (size_t i = 0; i < s_bloomMipChain.size(); i++)
        {
            auto& targetFramebuffer = s_bloomMipChain[i];
            targetFramebuffer->ClearAndUse();

            s_bloomDownsampleShader->SetUniform2f("u_ScreenRes", glm::vec2(currentSource->GetWidth(), currentSource->GetHeight()));
            BindTexture(currentSource, 0);
            s_bloomDownsampleShader->SetUniform1i("u_ScreenTex", 0);
            
            s_bloomDownsampleShader->SetUniform1i("u_MipLevel", static_cast<int>(i));
            s_bloomDownsampleShader->SetUniform1f("u_Threshold", 1.0f);
            s_bloomDownsampleShader->SetUniform1f("u_SoftThreshold", 0.5f);

            DrawArray(s_fullscreenQuad.GetVertexArray());
            targetFramebuffer->Unbind();
            currentSource = s_bloomMipChain[i]->GetColorTexture();
        }

        state.BlendMode = BlendMode::Additive;
        PushRenderState(state);

        BindShader(s_bloomUpsampleShader);
        s_bloomUpsampleShader->SetUniform1f("u_FilterRadius", 0.005f);

        // Upsample the other way around
        for (size_t i = s_bloomMipChain.size() - 1; i > 0; i--)
        {
            currentSource = s_bloomMipChain[i]->GetColorTexture();
            auto& targetFramebuffer = s_bloomMipChain[i - 1];

            targetFramebuffer->Use();

            BindTexture(currentSource, 0);
            s_bloomUpsampleShader->SetUniform1i("u_ScreenTex", 0);

            DrawArray(s_fullscreenQuad.GetVertexArray());
            targetFramebuffer->Unbind();
        }
    }

    void Renderer::RenderComposition()
    {
        RenderState state;
        state.BlendMode = BlendMode::None;
        state.CullMode = CullFace::None;
        state.DepthTest = false;
        state.DepthWrite = false;
        PushRenderState(state);

        s_mainFramebuffer->ClearAndUse();

        BindShader(s_compositionShader);

        // Color
        BindTexture(s_lightPassFramebuffer->GetColorTexture(), 0);
        s_compositionShader->SetUniform1i("sceneTexture", 0);

        // Bloom
        if (!s_bloomMipChain.empty())
        {
            BindTexture(s_bloomMipChain[0]->GetColorTexture(), 2);
            s_compositionShader->SetUniform1i("bloomTexture", 2);
        }
        s_compositionShader->SetUniform1f("bloomStrength", 0.04f);

        DrawArray(s_fullscreenQuad.GetVertexArray());

        s_mainFramebuffer->Unbind();
    }
}