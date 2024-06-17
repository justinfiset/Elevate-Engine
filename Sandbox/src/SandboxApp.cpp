#include <ElevateEngine.h>

#include "imgui/imgui.h"

#include "ElevateEngine/Renderer/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/include/GLFW/glfw3.h>

class DebugLayer : public Elevate::Layer
{
private:
    bool debugMenuActive = false;

    std::shared_ptr<Elevate::Shader> m_Shader;
    std::shared_ptr<Elevate::VertexArray> m_VertexArray;

    std::unique_ptr<Elevate::Texture> m_Texture1;
    std::unique_ptr<Elevate::Texture> m_Texture2;
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
       
        // creating textures from files
        m_Texture1.reset(Elevate::Texture::Create("container.jpg"));
        m_Texture2.reset(Elevate::Texture::Create("awesomeface.png"));

        std::shared_ptr<Elevate::IndexBuffer> indexBuffer(Elevate::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_Shader.reset(Elevate::Shader::CreateFromFiles("main.vert", "main.frag"));
        m_Shader->Bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // TODO MAKE THE CAM USE THE SCREEN WIDTH AND HEIGHT BY DEFAULT
        Elevate::Camera cam(80.0f, 1280, 720);
        m_Shader->SetUniformMatrix4fv("model", model);
        m_Shader->SetUniformMatrix4fv("view", cam.GetViewMatrix());
        m_Shader->SetUniformMatrix4fv("projection", cam.GetProjectionMatrix());

        // Binding textures // todo automatiser le tout
        m_Shader->SetUniform1i("texture1", 0);
        m_Shader->SetUniform1i("texture2", 1);
    }

    void OnRender() override {
        Elevate::Renderer::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f }); // Peut être facultatif car on s'en fou un peu au final
        Elevate::Renderer::Clear();

        Elevate::Renderer::BeginSceneFrame();
        m_Shader->Bind();
            
        // manually binding textures
        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

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