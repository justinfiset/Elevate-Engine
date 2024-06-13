#include <ElevateEngine.h>

#include "imgui/imgui.h"

class DebugLayer : public Elevate::Layer
{
private:
    bool debugMenuActive = false;

    std::shared_ptr<Elevate::Shader> m_Shader;
    std::shared_ptr<Elevate::VertexArray> m_VertexArray;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {
        float vertices[] = {
            // positions          // colors					// texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
           -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
           -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
        };

        std::shared_ptr<Elevate::VertexBuffer> vertexBuffer(Elevate::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout({
            { Elevate::ShaderDataType::Float3, "a_Position" },
            { Elevate::ShaderDataType::Float4, "a_Color" },
            { Elevate::ShaderDataType::Float2, "a_TexCord" }
        });

        // Create and bind vertex array
        m_VertexArray.reset(Elevate::VertexArray::Create());
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        // Creation of the index buffer
        uint32_t indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        std::shared_ptr<Elevate::IndexBuffer> indexBuffer(Elevate::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_Shader.reset(Elevate::Shader::CreateFromFiles("main.vert", "main.frag"));
    }

    void OnRender() override {
        Elevate::Renderer::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f }); // Peut être facultatif car on s'en fou un peu au final
        Elevate::Renderer::Clear();

        Elevate::Renderer::BeginSceneFrame();
        m_Shader->Bind();

        /*
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        */

        Elevate::Renderer::SubmitMeshes(m_VertexArray);
        m_VertexArray->Bind();
        Elevate::Renderer::EndSceneFrame();
    }

    void OnUpdate() override
    {
        if (Elevate::Input::IsKeyDown(EE_KEY_TAB))
        {
            debugMenuActive = !debugMenuActive;
        }
    }

    void OnEvent(Elevate::Event& event) override
    {
    }

    void OnImGuiRender() override
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if (!debugMenuActive) return;

        ImGui::Begin("Elevate Engine", &debugMenuActive, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Hide")) { debugMenuActive = false; }
                if (ImGui::MenuItem("Exit")) { exit(0); }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::Text("Elevate Engine - Debugging");  
        ImGui::Spacing();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
};

class Sandbox : public Elevate::Application
{
public:
    Sandbox() 
    {
        PushLayer(new DebugLayer());
    }

    ~Sandbox()
    {

    }
};

Elevate::Application* Elevate::CreateApplication() {
    return new Sandbox();
}