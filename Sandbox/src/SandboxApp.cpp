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
    // todo make an ortographic and perspective cam class
    Elevate::Camera cam = Elevate::Camera(60.0f);

    std::unique_ptr<Elevate::Model> m_Model;
    std::vector<std::unique_ptr<Elevate::Model>> m_LightModels;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {   
        // TODO in depth testing pour voir si les texures se load tous comme du monde avec le nouveau systeme
        m_Model = std::make_unique<Elevate::Model>(Elevate::Model("backpack.obj"));

        m_Shader.reset(Elevate::Shader::CreateFromFiles("main.vert", "main.frag"));
        m_Shader->Bind();

        m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(0.0f, 0.0f, -3.0f));
        //m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        /// MATRIX PRINTING
        //for (int i = 0; i < 4; ++i) {
        //    std::string line;
        //    for (int j = 0; j < 4; ++j) {
        //        line.append(std::to_string(m_ModelMatrix[i][j]) + " ");
        //    }
        //    EE_TRACE(line);
        //}
        
        glm::vec3 pointLightPositions[] = {
            glm::vec3(-2.0f,  0.0f, 0.0f),
            glm::vec3(-1.0f,  0.0f, 0.0f),
            glm::vec3(0.0f,  0.0f, 0.0f),
            glm::vec3(1.0f,  0.0f, 0.0f)
        };

        for (size_t i = 0; i <= pointLightPositions->length(); i++)
        {
            EE_TRACE(i);
            m_LightModels.push_back(std::make_unique<Elevate::Model>(Elevate::Model("cube.obj")));
            m_LightModels[i]->GetMatrix() = glm::translate(m_LightModels[i]->GetMatrix(), pointLightPositions[i]);
        }

        // Lighting ////////////////////////////////////////
        // TODO CREATE A LIGHT STRUCT OR CLASS (something like material bellow)
        m_Shader->SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        m_Shader->SetUniform3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f); // darken diffuse light a bit
        m_Shader->SetUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // point light 1
        m_Shader->SetUniform3f("pointLights[0].position", pointLightPositions[0]);
        m_Shader->SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[0].constant", 1.0f);
        m_Shader->SetUniform1f("pointLights[0].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[0].quadratic", 0.032f);
        // point light 2
        m_Shader->SetUniform3f("pointLights[1].position", pointLightPositions[1]);
        m_Shader->SetUniform3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[1].constant", 1.0f);
        m_Shader->SetUniform1f("pointLights[1].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[1].quadratic", 0.032f);
        // point light 3
        m_Shader->SetUniform3f("pointLights[2].position", pointLightPositions[2]);
        m_Shader->SetUniform3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[2].constant", 1.0f);
        m_Shader->SetUniform1f("pointLights[2].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[2].quadratic", 0.032f);
        // point light 4
        m_Shader->SetUniform3f("pointLights[3].position", pointLightPositions[3]);
        m_Shader->SetUniform3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[3].constant", 1.0f);
        m_Shader->SetUniform1f("pointLights[3].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[3].quadratic", 0.032f);
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
        // sprint
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
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu à la place
        m_Model->Draw(m_Shader);
        for (size_t i = 0; i < m_LightModels.size(); i++)
        {
            m_LightModels[i]->Draw(m_Shader);
        }
        //Elevate::Renderer::DrawStack(m_Shader); // WRONG // TODO redo and use
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
            // calculating the offset
            float xpos = Elevate::Input::GetMouseX();
            float ypos = Elevate::Input::GetMouseY();
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
            // setting the last mouse pos
            lastX = xpos;
            lastY = ypos;
            // adjusting to a sensitivity
            const float sensitivity = 0.5f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            // clamp between 89 and -89 deg.
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

        // Obtenez la taille de l'écran / de la fenêtre principale
        ImVec2 mainViewportPos = ImGui::GetMainViewport()->Pos;
        ImVec2 mainViewportSize = ImGui::GetMainViewport()->Size;

        // Définissez la taille et la position de la fenêtre
        ImVec2 windowSize = ImVec2(mainViewportSize.x / 4, mainViewportSize.y); // 200 est la hauteur de la fenêtre en pixels
        ImVec2 windowPos = ImVec2(mainViewportPos.x, mainViewportPos.y + mainViewportSize.y - windowSize.y);
        // Définissez les drapeaux pour rendre la fenêtre non déplaçable, non redimensionnable et sans barre de titre
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

        // Appliquez la taille et la position avant de créer la fenêtre
        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(windowPos);
        
        // Créez la fenêtre
        if (ImGui::Begin("Bottom Dock Window", nullptr, windowFlags))
        {
            ImGui::Text("Elevate Engine");
            ImGui::Separator();
            //ImGui::InputFloat3("Model Position", &m_ModelMatrix[3][0]);

            //ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", m_ModelMatrix[3].x, m_ModelMatrix[3].y, m_ModelMatrix[3].z);
            //ImGui::End();
        }

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