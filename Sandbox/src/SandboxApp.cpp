#include <ElevateEngine.h>

#include "imgui/imgui.h"
#include "ImGuizmo.h"

#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"


// MATHS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/include/GLFW/glfw3.h>

#include "ElevateEngine/Renderer/Cubemap.h"
#include "ElevateEngine/Renderer/Light/DirectionalLight.h"
#include "ElevateEngine/Renderer/Light/PointLight.h"

#include "tinyfiledialogs.h"

#include "ElevateEngine/ImGui/ImGuiTheme.h"
#include <ElevateEngine/Renderer/FrameBuffer.h>

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
    std::unique_ptr<Elevate::Cubemap> m_Cubemap;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {   
        // TODO in depth testing pour voir si l es texures se load tous comme du monde avec le nouveau systeme
        m_Model = std::make_unique<Elevate::Model>(Elevate::Model("backpack.obj"));

        uint32_t glslVersion = 330;
        uint32_t glslPointLightCount = 1;

        std::string glslVesionDefine = "#version " + std::to_string(glslVersion);
        std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(glslPointLightCount);
        m_Shader.reset(Elevate::Shader::CreateFromFiles(
            "shader/main.vert",
            "shader/main.frag",
            glslVesionDefine,
            glslVesionDefine + "\n" + glslPointLightCountDefine
        ));


        // TODO REMOVE FOLLOWING LINES
        //std::string paths[6] =
        //{
        //    "skybox/graycloud_rt.jpg",
        //    "skybox/graycloud_lf.jpg",
        //    "skybox/graycloud_up.jpg",
        //    "skybox/graycloud_dn.jpg",
        //    "skybox/graycloud_ft.jpg",
        //    "skybox/graycloud_bk.jpg"
        //};
        m_Cubemap.reset(Elevate::Cubemap::CreateFromFile("cubemap/default.sky"));

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

        m_Shader->Bind();

        Elevate::DirectionalLight dirLight;
        dirLight.Use(m_Shader);

        
        glm::vec3 pointLightPositions[] = {
            glm::vec3(-2.0f,  0.0f, 0.0f),
            glm::vec3(-1.0f,  0.0f, 0.0f),
            glm::vec3(0.0f,  0.0f, 0.0f),
            glm::vec3(1.0f,  0.0f, 0.0f)
        };

        // SETTING THE MATERIAL /////////////////////////////////////////
        Elevate::Material material /*
        (
            { 0.5f, 0.5f, 0.5f }, // Ambient
            { 0.5f, 0.5f, 0.5f }, // Diffuse
            { 0.5f, 0.5f, 0.5f }, // Specular
            1.0f                  // Shine
        )*/;
        // TODO faire l'inverse? : m_Shader.UseMaterial(material) ???????
        material.Use(m_Shader);

        //// # light 1
        m_Shader->SetUniform3f("pointLights[0].position", pointLightPositions[0]);
        m_Shader->SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[0].constant", 1.0f);    
        m_Shader->SetUniform1f("pointLights[0].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[0].quadratic", 0.032f);
        //// point light 2
        //m_Shader->SetUniform3f("pointLights[1].position", pointLightPositions[1]);
        //m_Shader->SetUniform3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        //m_Shader->SetUniform3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        //m_Shader->SetUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        //m_Shader->SetUniform1f("pointLights[1].constant", 1.0f);
        //m_Shader->SetUniform1f("pointLights[1].linear", 0.09f);
        //m_Shader->SetUniform1f("pointLights[1].quadratic", 0.032f);
        //// point light 3
        //m_Shader->SetUniform3f("pointLights[2].position", pointLightPositions[2]);
        //m_Shader->SetUniform3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        //m_Shader->SetUniform3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        //m_Shader->SetUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        //m_Shader->SetUniform1f("pointLights[2].constant", 1.0f);
        //m_Shader->SetUniform1f("pointLights[2].linear", 0.09f);
        //m_Shader->SetUniform1f("pointLights[2].quadratic", 0.032f);
        //// point light 4
        //m_Shader->SetUniform3f("pointLights[3].position", pointLightPositions[3]);
        //m_Shader->SetUniform3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
            //m_Shader->SetUniform3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
            //m_Shader->SetUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        //m_Shader->SetUniform1f("pointLights[3].constant", 1.0f);
        //m_Shader->SetUniform1f("pointLights[3].linear", 0.09f);
        //m_Shader->SetUniform1f("pointLights[3].quadratic", 0.032f);
        //////////////////////////////////////////////////////
        

        /////////////////////////////////////////////////////////////////

        //Elevate::Renderer::SubmitModel(*m_Model); /// WRONG

        // TODO impl dans un API a part entiere
        //// Boîte de dialogue pour choisir un fichier
        //const char* filePath = tinyfd_openFileDialog(
        //    "Find a skybox",
        //    "",
        //    0,
        //    NULL,
        //    NULL,
        //    0
        //);

        // TODO impl dans un API a part entiere
        //tinyfd_messageBox(
        //    "Erreur",
        //    "Une erreur s'est produite lors de l'exécution.",
        //    "ok",
        //    "error",
        //    1
        //);
    }

    void ShowImGuiOpenGLWindow(GLuint textureColorbuffer, int width, int height) {
        ImGui::Begin("OpenGL View");
        ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2(width, height));
        ImGui::End();
    }

    void OnRender() override {
        glm::mat4 view = glm::mat4(glm::mat3(cam.GenViewMatrix()));

        m_Cubemap->SetProjectionMatrix(cam.GetProjectionMatrix());
        m_Cubemap->SetViewMatrix(view);
        m_Cubemap->Draw();

        //Elevate::Renderer::BeginSceneFrame(m_Shader);

        m_Shader->Bind();
        m_Shader->SetUniform3f("camPos", cam.GetPoition()->x, cam.GetPoition()->y, cam.GetPoition()->z);
        m_Shader->SetUniformMatrix4fv("viewProj", cam.GenViewProjectionMatrix());
        // On soumet les models et on les affiches en dessinant la stack
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu à la place
        m_Model->Draw(m_Shader);
        //Elevate::Renderer::DrawStack(m_Shader); // WRONG // TODO redo and use
        //Elevate::Renderer::EndSceneFrame(m_Shader);
    }

    void OnUpdate() override
    {
        // TODO remove at some point cuz useless    
        float mod = 2.0f * Elevate::Time::GetDeltaTime();
        if (Elevate::Input::IsKeyPressed(EE_KEY_LEFT))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(-mod, 0.0f, 0.0f));
        }
        else if (Elevate::Input::IsKeyPressed(EE_KEY_RIGHT))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(mod, 0.0f, 0.0f));
        }
        if (Elevate::Input::IsKeyPressed(EE_KEY_UP))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(0.0f, 0.0f, -mod));
        }
        else if (Elevate::Input::IsKeyPressed(EE_KEY_DOWN))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(0.0f, 0.0f, mod));
        }

        if (Elevate::Input::IsKeyDown(EE_KEY_TAB))
        {
            debugMenuActive = !debugMenuActive;
        }

        // Camera movement ///////////////////////////////
        // sprint
        float baseCamSpeed = 1.0f;
        if (Elevate::Input::IsKeyPressed(EE_KEY_LEFT_SHIFT))
        {
            baseCamSpeed = 2.5f;
        }
        else
        {
            baseCamSpeed = 0.5f;
        }
        float cameraSpeed = baseCamSpeed * Elevate::Time::GetDeltaTime();

        if (Elevate::Input::IsKeyPressed(EE_KEY_W))
            cam.GetTransform()->position += cameraSpeed * cam.m_front;
        if (Elevate::Input::IsKeyPressed(EE_KEY_S))
            cam.GetTransform()->position -= cameraSpeed * cam.m_front;
        if (Elevate::Input::IsKeyPressed(EE_KEY_D))
            cam.GetTransform()->position -= cameraSpeed * cam.m_right;
        if (Elevate::Input::IsKeyPressed(EE_KEY_A))
            cam.GetTransform()->position += cameraSpeed * cam.m_right;

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

            cam.GetTransform()->rotation.y += xoffset;
            cam.GetTransform()->rotation.x += yoffset;

            // clamp between 89 and -89 deg.
            if (cam.GetTransform()->rotation.x > 89.0f)
                cam.GetTransform()->rotation.x = 89.0f;
            if (cam.GetTransform()->rotation.x < -89.0f)
                cam.GetTransform()->rotation.x = -89.0f;

            cam.UpdateCameraVectors();
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
        // TODO REMOVE DANS LE FUTUR... ON AFFICHE LE UI?
        // ON PÊUT APPUYER SUR TAB POUR LE FERMER SI ON VEUX VOIR LE BACKGROUND
        //if (!debugMenuActive) return;

        ImGuiIO& io = ImGui::GetIO();


        // Setup the docking space to snap widgets int he window
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
        ImGui::PopStyleVar(3);
        ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f)/*ImGuiDockNodeFlags_NoDockingInCentralNode*/);
        //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
        ImGui::End();

        // TODO IMPL. FEATURES
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // TODO handle projects
                if (ImGui::MenuItem("New project")) { /* Action pour créer une nouvelle scène */ }
                if (ImGui::MenuItem("Open project")) { /* Action pour ouvrir un fichier */ }
                if (ImGui::MenuItem("Save")) { /* Action pour sauvegarder */ }
                if (ImGui::MenuItem("Exit")) { exit(0); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                // TODO impl. action stack
                if (ImGui::MenuItem("Undo")) { /* Action pour annuler */ }
                if (ImGui::MenuItem("Redo")) { /* Action pour refaire */ }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Objects"))
            {
                // TODO impl. instatntiating

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Get the pos and size of the viewport
        ImVec2 mainViewportPos = ImGui::GetMainViewport()->Pos;
        ImVec2 mainViewportSize = ImGui::GetMainViewport()->Size;


        ////////////////////////////////////
        //// TODO PUT IN SEPERATE FILES ASAP
        //ImGui::Begin("Hierarchy");

        //for (auto& object : sceneObjects) // Supposons que sceneObjects soit une liste d'objets dans la scène
        //{
        //    if (ImGui::TreeNode(object.name.c_str()))
        //    {
        //        // Sélectionnez l'objet
        //        if (ImGui::IsItemClicked()) {
        //            selectedObject = &object;
        //        }

        //        ImGui::TreePop();
        //    }
        //}

        //// Drag and Drop pour réorganiser
        //if (ImGui::BeginDragDropSource())
        //{
        //    ImGui::SetDragDropPayload("DND_OBJECT", &selectedObject, sizeof(selectedObject));
        //    ImGui::Text("Dragging %s", selectedObject->name.c_str());
        //    ImGui::EndDragDropSource();
        //}

        //if (ImGui::BeginDragDropTarget())
        //{
        //    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_OBJECT"))
        //    {
        //        // Logique pour réorganiser les objets dans la hiérarchie
        //    }
        //    ImGui::EndDragDropTarget();
        //}

        //ImGui::End();
        ////////////
        //ImGui::Begin("Scene");

        //ImVec2 windowSize = ImGui::GetContentRegionAvail();  // Taille de la fenêtre ImGui
        //ImGui::Image((void*)(intptr_t)framebufferTexture, windowSize);

        //// Ajouter la gestion des interactions (zoom, déplacement, etc.)
        //if (ImGui::IsItemClicked()) // Selecting an object
        //{
        //}

        //ImGui::End();
        ///////////////////////
        //ImGui::Begin("Analyse");

        //if (selectedObject != nullptr)
        //{
        //    ImGui::Text("Selected Object: %s", selectedObject->name.c_str());

        //    ImGui::InputText("Name", selectedObject->name.data(), selectedObject->name.size());
        //    ImGui::SliderFloat3("Position", glm::value_ptr(selectedObject->position), -10.0f, 10.0f);
        //    ImGui::SliderFloat3("Rotation", glm::value_ptr(selectedObject->rotation), -180.0f, 180.0f);
        //    ImGui::SliderFloat3("Scale", glm::value_ptr(selectedObject->scale), 0.1f, 10.0f);
        //}

        //ImGui::End();
        
        // SCENE VIEW /////////////////
        ImGui::Begin("Scene View");

        // we access the ImGui window size
        float window_width = ImGui::GetContentRegionAvail().x;
        float window_height = ImGui::GetContentRegionAvail().y;

        // Keeping the aspect ratio for the scene view
        // Aspect ratio de 16:9
        const float arX = 16;
        const float arY = 9;
        const float qtX = window_width / arX;
        const float qtY = window_height / arY;
        if (qtX < qtY)
            window_height = qtX * arY;
        else
            window_width = qtY * arX;

        std::shared_ptr<Elevate::FrameBuffer> frameBuffer = Elevate::Application::Get().GetFrameBuffer();
        // we rescale the framebuffer to the actual window size here and reset the glViewport 
        frameBuffer->Rescale(window_width, window_height);
        glViewport(0, 0, window_width, window_height); // TODO remove from here asap

        // we get the screen position of the window
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::Image((void*)(intptr_t)frameBuffer->GetTextureId(), ImVec2(window_width, window_height), ImVec2(0, 1), ImVec2(1, 0));

        // ImGuizmo //////////////////////////////////////////
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
        ImGuizmo::SetOrthographic(false); // TODO SET DINAMICLY
        ImGuizmo::SetRect(pos.x, pos.y, window_width, window_height);

        glm::mat4 cameraProjection = cam.GetProjectionMatrix();
        glm::mat4 cameraView = cam.GenViewMatrix();
        glm::mat4 entityMatrix = m_Model->GetMatrix();

        // TODO SET VIA BUTTONS
        ImGuizmo::Manipulate(
            glm::value_ptr(cameraView),
            glm::value_ptr(cameraProjection),
            ImGuizmo::TRANSLATE,    // Change to ROTATE or SCALE as needed
            ImGuizmo::WORLD,        // Change to WORLD if needed
            glm::value_ptr(entityMatrix)
        );
        m_Model->SetMatrix(entityMatrix);

        ImGui::End();
        ////////////////////////////////////////////////////////

 
        if (ImGui::Begin("Debugging"))
        {
            ImGui::Text("Elevate Engine - Debugging");  
            ImGui::Separator();
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            
            ImGui::End();
        }
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