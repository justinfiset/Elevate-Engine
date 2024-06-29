#include <ElevateEngine.h>

#include "imgui/imgui.h"

#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"

// MATHS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/include/GLFW/glfw3.h>

class DebugLayer : public Elevate::Layer
{
private:
    bool debugMenuActive = false;

    std::shared_ptr<Elevate::Shader> m_Shader;

    // Camera specific stuff
    float lastX, lastY;
    bool followCursor = false;
    Elevate::Camera cam = Elevate::Camera(60.0f);

    std::unique_ptr<Elevate::Model> m_Model;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {
        // TODO abstract somewhere

        //m_Model = std::make_unique<Elevate::Model>(Elevate::Model("Lowpoly_Notebook_2.obj"));
        m_Model = std::make_unique<Elevate::Model>(Elevate::Model("backpack.obj"));

        m_Shader.reset(Elevate::Shader::CreateFromFiles("main.vert", "main.frag"));
        m_Shader->Bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // TODO MAKE THE CAM USE THE SCREEN WIDTH AND HEIGHT BY DEFAULT
        m_Shader->SetUniformMatrix4fv("model", model); // set the model matrix

        // Lighting ////////////////////////////////////////
        // TODO CREATE A LIGHT STRUCT OR CLASS (something like material bellow)
        m_Shader->SetUniform3f("light.position", 0.5f, 0.5f, 0.5f);
        m_Shader->SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
        m_Shader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        m_Shader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
        // -> Specular /////////////////////////////////////
        ////////////////////////////////////////////////////

        // SETTING THE MATERIAL /////////////////////////////////////////
        Elevate::Material material;
        material.Use(m_Shader);
        /////////////////////////////////////////////////////////////////

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

        m_Shader->SetUniform3f("camPos", cam.GetPoition()->x, cam.GetPoition()->y, cam.GetPoition()->z);
        m_Shader->SetUniformMatrix4fv("viewProj", cam.GenViewProjectionMatrix());

        // On soumet les models et on les affiches en dessinant la stack
        //m_VertexArray->Bind();
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu à la place
        m_Model->Draw(m_Shader);
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
            // TODO fix mouse lag drag
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