#include <ElevateEngine.h>

#include "imgui/imgui.h"

#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"

// MATHS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DebugLayer : public Elevate::Layer
{
private:
    bool debugMenuActive = false;

    std::shared_ptr<Elevate::Shader> m_Shader;

    std::unique_ptr<Elevate::Texture> m_Texture1;
    std::unique_ptr<Elevate::Texture> m_Texture2;

    // Camera specific stuff
    float lastX, lastY;
    bool followCursor = false;
    Elevate::Camera cam = Elevate::Camera(60.0f);

    std::unique_ptr<Elevate::Model> m_Model;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {
        //float vertices[] = {
        //    // Position, normal, texture Cord
        //    -0.5f, 0.0f,  0.5f,      0.0f, -1.0f, 0.0f,     0.0f, 0.0f, // Bottom side
        //    -0.5f, 0.0f, -0.5f,      0.0f, -1.0f, 0.0f,     0.0f, 5.0f, // Bottom side
        //     0.5f, 0.0f, -0.5f,      0.0f, -1.0f, 0.0f,     5.0f, 5.0f, // Bottom side
        //     0.5f, 0.0f,  0.5f,      0.0f, -1.0f, 0.0f,     5.0f, 0.0f, // Bottom side

        //    -0.5f, 0.0f,  0.5f,     -0.8f, 0.5f,  0.0f,     0.0f, 0.0f, // Left Side
        //    -0.5f, 0.0f, -0.5f,     -0.8f, 0.5f,  0.0f,     5.0f, 0.0f, // Left Side
        //     0.0f, 0.8f,  0.0f,     -0.8f, 0.5f,  0.0f,     2.5f, 5.0f, // Left Side

        //    -0.5f, 0.0f, -0.5f,      0.0f, 0.5f, -0.8f,     5.0f, 0.0f, // Non-facing side
        //     0.5f, 0.0f, -0.5f,      0.0f, 0.5f, -0.8f,     0.0f, 0.0f, // Non-facing side
        //     0.0f, 0.8f,  0.0f,      0.0f, 0.5f, -0.8f,     2.5f, 5.0f, // Non-facing side

        //     0.5f, 0.0f, -0.5f,      0.8f, 0.5f,  0.0f,     0.0f, 0.0f, // Right side
        //     0.5f, 0.0f,  0.5f,      0.8f, 0.5f,  0.0f,     5.0f, 0.0f, // Right side
        //     0.0f, 0.8f,  0.0f,      0.8f, 0.5f,  0.0f,     2.5f, 5.0f, // Right side

        //     0.5f, 0.0f,  0.5f,      0.0f, 0.5f,  0.8f,     5.0f, 0.0f, // Facing side
        //    -0.5f, 0.0f,  0.5f,      0.0f, 0.5f,  0.8f,     0.0f, 0.0f, // Facing side
        //     0.0f, 0.8f,  0.0f,      0.0f, 0.5f,  0.8f,     2.5f, 5.0f  // Facing side
        //};

        //uint32_t indices[] =
        //{
        //    0, 1, 2, // Bottom side
        //    0, 2, 3, // Bottom side
        //    4, 6, 5, // Left side
        //    7, 9, 8, // Non-facing side
        //    10, 12, 11, // Right side
        //    13, 15, 14 // Facing side
        //};

        ////////////////////////////////////////////////////////////////////////////
        //std::shared_ptr<Elevate::VertexBuffer> m_VertexBuffer;
        //std::shared_ptr<Elevate::IndexBuffer> m_IndexBuffer;
        //// Creating the Layout and the VertexBuffer
        //m_VertexBuffer.reset(Elevate::VertexBuffer::Create(vertices, sizeof(vertices)));
        //// Creating the layout sent to the shader and the layout of the buffer
        //m_VertexBuffer->SetLayout({ // The layout is based on the Vertex struct (see Vertex.h)
        //    { Elevate::ShaderDataType::Float3, "a_Position" },
        //    { Elevate::ShaderDataType::Float3, "a_Normal" },
        //    { Elevate::ShaderDataType::Float2, "a_TexCord" },
        //});
        ////Creating the IndexBuffer (containing indices)
        //m_IndexBuffer.reset(Elevate::IndexBuffer::Create(&indices, sizeof(indices) / sizeof(uint32_t)));
        ////Vertex Array Creation
        //m_VertexArray.reset(Elevate::VertexArray::Create());
        //m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        //m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        ////////////////////////////////////////////////////////////////////////////

        //m_VertexArray->AddVertexBuffer(m_Mesh->GetVertexBuffer());
        //m_VertexArray->SetIndexBuffer(m_Mesh->GetIndexBuffer());

        // Create and bind vertex array
        // TODO abstract somewhere

        m_Model = std::make_unique<Elevate::Model>(Elevate::Model("cube.obj"));
        //m_Model = std::make_unique<Elevate::Model>(Elevate::Model("backpack.fbx"));

        // creating textures from files
        m_Texture1.reset(Elevate::Texture::Create("container.jpg"));
        m_Texture2.reset(Elevate::Texture::Create("awesomeface.png"));

        m_Shader.reset(Elevate::Shader::CreateFromFiles("main.vert", "main.frag"));
        m_Shader->Bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // TODO MAKE THE CAM USE THE SCREEN WIDTH AND HEIGHT BY DEFAULT
        m_Shader->SetUniformMatrix4fv("model", model); // set the model matrix

        // Lighting ////////////////////////////////////////
        float ambiantStrenght = 0.1f;
        glm::vec3 ambiantLightColor(1.0f, 1.0f, 1.0f);
        ambiantLightColor *= ambiantStrenght;
        m_Shader->SetUniform3f("lightPos", 0.5f, 0.5f, 0.5f);
        m_Shader->SetUniform3f("lightColor", ambiantLightColor.r, ambiantLightColor.g, ambiantLightColor.b);
        // -> Specular /////////////////////////////////////
        ////////////////////////////////////////////////////

        // TODO pouvoir fournir des vecteurs directement au shader

        // Binding textures // todo automatiser le tout en fournissant le shader
        m_Shader->SetUniform1i("texture1", 0);
        m_Shader->SetUniform1i("texture2", 1);

        //Elevate::Renderer::SubmitModel(*m_Model); /// WRONG
    }

    void OnRender() override {
        Elevate::Renderer::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f }); // Peut être facultatif car on s'en fou un peu au final
        Elevate::Renderer::Clear();

        Elevate::Renderer::BeginSceneFrame(m_Shader);

        glm::vec3 cameraFront = cam.GetFront();
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        // Camera movement ///////////////////////////////
        float baseCamSpeed = 1.0f;
        if (Elevate::Input::IsKeyPressed(EE_KEY_LEFT_SHIFT))
            baseCamSpeed = 2.5f;
        else
            baseCamSpeed = 0.5f;
        float cameraSpeed = baseCamSpeed * Elevate::Time::GetDeltaTime();

        if (Elevate::Input::IsKeyPressed(EE_KEY_W))
            cam.GetTransform()->position += cameraSpeed * cameraFront;
        if (Elevate::Input::IsKeyPressed(EE_KEY_S))
            cam.GetTransform()->position -= cameraSpeed * cameraFront;
        if (Elevate::Input::IsKeyPressed(EE_KEY_A))
            cam.GetTransform()->position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (Elevate::Input::IsKeyPressed(EE_KEY_D))
            cam.GetTransform()->position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //EE_CORE_TRACE("{0},{1},{2}", cam.GetPoition()->x, cam.GetPoition()->y, cam.GetPoition()->z);

        m_Shader->SetUniform3f("camPos", cam.GetPoition()->x, cam.GetPoition()->y, cam.GetPoition()->z);
        m_Shader->SetUniformMatrix4fv("viewProj", cam.GenViewProjectionMatrix());

        // manually binding textures based on the texture index
        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

        // On soumet les models et on les affiches en dessinant la stack
        //m_VertexArray->Bind();
        m_Model->Draw();
        //Elevate::Renderer::DrawStack(m_Shader); // WRONG
        Elevate::Renderer::EndSceneFrame(m_Shader);
    }

    void OnUpdate() override
    {
        if (Elevate::Input::IsKeyDown(EE_KEY_TAB))
        {
            debugMenuActive = !debugMenuActive;
        }

        if (followCursor)
        {
            float xpos = Elevate::Input::GetMouseX();
            float ypos = Elevate::Input::GetMouseY();
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
            lastX = xpos;
            lastY = ypos;

            const float sensitivity = 0.5f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            if (cam.GetRotation()->x > 89.0f)
                cam.GetRotation()->x = 89.0f;
            if (cam.GetRotation()->x < -89.0f)
                cam.GetRotation()->x = -89.0f;

            if (xoffset != 0 && yoffset != 0)
            {
                cam.GetTransform()->rotation.y += xoffset;
                cam.GetTransform()->rotation.x += yoffset;
            }
        }
    }

    void OnEvent(Elevate::Event& event) override
    {
        if (event.GetEventType() == Elevate::EventType::MouseMoved)
        {
            Elevate::MouseMovedEvent& mouseEvent = dynamic_cast<Elevate::MouseMovedEvent&>(event);
        }

        if (event.IsInCategory(Elevate::EventCategoryMouseButton))
        {
            if (Elevate::Input::IsMouseButtonDown(EE_MOUSE_BUTTON_RIGHT))
            {
                followCursor = true;
                lastX = Elevate::Input::GetMouseX();
                lastY = Elevate::Input::GetMouseY();
            }
            else if (Elevate::Input::IsMouseButtonUp(EE_MOUSE_BUTTON_RIGHT))
            {
                followCursor = false;
            }
        }

        //if (event.GetEventType() == Elevate::EventType::MouseButtonPressed)
        //{
        //    Elevate::MouseButtonPressedEvent& mouseEvent = dynamic_cast<Elevate::MouseButtonPressedEvent&>(event);
        //    EE_CORE_TRACE(mouseEvent.ToString());
        //}
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