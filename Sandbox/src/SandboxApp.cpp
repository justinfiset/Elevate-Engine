#include <ElevateEngine.h>

#include "imgui/imgui.h"
#include "ImGuizmo.h"

#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"


// MATHS
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/include/GLFW/glfw3.h>

#include "ElevateEngine/Renderer/Cubemap.h"
#include "ElevateEngine/Renderer/Light/DirectionalLight.h"
#include "ElevateEngine/Renderer/Light/PointLight.h"

#include "tinyfiledialogs.h"

#include "ElevateEngine/ImGui/CustomImGuiCommand.h"
#include "ElevateEngine/ImGui/ImGuiTheme.h"
#include <ElevateEngine/Renderer/FrameBuffer.h>

#include "ElevateEngine/Scene/Scene.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Transform.h"

class DebugLayer : public Elevate::Layer
{
private:
    std::shared_ptr<Elevate::Shader> m_Shader;

    // Camera specific stuff
    float lastX, lastY;
    bool followCursor = false;

    // TODO make an ortographic and perspective cam class
    // Camera
    std::shared_ptr<Elevate::GameObject> m_CameraObject;
    Elevate::Camera cam;

    // Grid
    std::shared_ptr<Elevate::Shader> m_GridShader; 
    std::shared_ptr<Elevate::GameObject> m_GridObject;
    std::unique_ptr<Elevate::Cubemap> m_Cubemap;
    
    std::shared_ptr<Elevate::GameObject> m_DemoObject;

    std::shared_ptr<Elevate::GameObject> m_PointLightObject;

    std::shared_ptr<Elevate::GameObject> m_SelectedObject;

    std::shared_ptr<Elevate::Scene> m_Scene;
    std::shared_ptr<Elevate::Scene> m_EditorScene;

    // Aspect Ratio Selection
    int aspectRatioValue = 3;
    glm::ivec2 aspectRatioSettings[5] =
    {
        { 3, 2 },
        { 4, 3 },
        { 5, 4 },
        { 16, 9 },
        { 16, 10 }
    };

    // Editor tool option
    int m_CurrentEditorTool = 7;
public:
    // TODO Change from debug to edtitor
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {   
        // Scenes creation
        m_Scene = std::make_shared<Elevate::Scene>("Demo Scene");
        m_EditorScene = std::make_shared<Elevate::Scene>("Editor");

        uint32_t glslVersion = 330;
        uint32_t glslPointLightCount = 1;

        std::string glslVesionDefine = "#version " + std::to_string(glslVersion);
        std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(glslPointLightCount);

        // Setup the grid shader ///////////////////////////
        m_GridShader.reset(Elevate::Shader::CreateFromFiles(
            "shader/grid.vert",
            "shader/grid.frag"
        ));
        m_GridShader->Bind();
        m_GridShader->SetUniform4f("lineColor", { 0.9, 0.9, 0.9, 0.5 });
        m_GridShader->SetUniform4f("backgroundColor", { 0.6, 0.6, 0.6, 0.025 });
        //////////////////////////////////////////////

        m_Shader.reset(Elevate::Shader::CreateFromFiles(
            "shader/main.vert",
            "shader/main.frag",
            glslVesionDefine,
            glslVesionDefine + "\n" + glslPointLightCountDefine
        ));

        m_Cubemap.reset(Elevate::Cubemap::CreateFromFile("cubemap/default.sky"));

        /// MATRIX PRINTING
        //for (int i = 0; i < 4; ++i) {
        //    std::string line;
        //    for (int j = 0; j < 4; ++j) {
        //        line.append(std::to_string(m_ModelMatrix[i][j]) + " ");
        //    }
        //    EE_TRACE(line);
        //}

        // TODO impl dans un API a part entiere
        //// Bo�te de dialogue pour choisir un fichier
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
        //    "Une erreur s'est produite lors de l'ex�cution.",
        //    "ok",
        //    "error",
        //    1
        //);


        // Backpack
        m_DemoObject = std::make_shared<Elevate::GameObject>("Backpack");
        m_Scene->AddRootObject(m_DemoObject);
        Elevate::Model& demoModel = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        demoModel.SetShader(m_Shader);
        m_DemoObject->SetPosition({ 0.0f, 0.0f, -3.0f });

        // point light
        m_PointLightObject = std::make_shared<Elevate::GameObject>("Point Light");
        m_PointLightObject->SetPosition({ -2.0f, 0.0f, 0.0f });
        m_Scene->AddRootObject(m_PointLightObject);

        // Grid
        m_GridObject = std::make_shared<Elevate::GameObject>("Editor Grid");
        m_EditorScene->AddRootObject(m_GridObject);
        Elevate::Model& gridModel = m_GridObject->AddComponent<Elevate::Model>("model/plane.obj");
        gridModel.SetShader(m_GridShader);
        m_GridObject->SetScale({ 50, 50, 50 });

        // Editor Camera
        m_CameraObject = std::make_shared<Elevate::GameObject>("Editor Camera");
        m_EditorScene->AddRootObject(m_CameraObject);
        cam = m_CameraObject->AddComponent<Elevate::Camera>(60.0f);

        m_Shader->Bind();

        Elevate::DirectionalLight dirLight;
        m_Shader->UseDirLight(&dirLight);

        // SETTING THE MATERIAL /////////////////////////////////////////
        Elevate::Material material /*
        (
            { 0.5f, 0.5f, 0.5f }, // Ambient
            { 0.5f, 0.5f, 0.5f }, // Diffuse
            { 0.5f, 0.5f, 0.5f }, // Specular
            1.0f                  // Shine
        )*/;
        m_Shader->UseMaterial(&material);

        // todo envoyer dans la classe pointlight
        // Set avoir le composant que l'on va cr�er
        m_Shader->SetUniform3f("pointLights[0].position", m_PointLightObject->GetPosition());
        m_Shader->SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[0].constant", 1.0f);
        m_Shader->SetUniform1f("pointLights[0].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[0].quadratic", 0.032f);
    }

    // TODO ajouter un icon de point light qui suit avec imgui la point light
    void OnRender() override {
        glm::mat4 view = glm::mat4(glm::mat3(cam.GenViewMatrix()));
        glm::vec3 camPos = m_CameraObject->GetPosition();

        m_Cubemap->SetProjectionMatrix(cam.GetProjectionMatrix());
        m_Cubemap->SetViewMatrix(view);
        m_Cubemap->Draw();

        //Elevate::Renderer::BeginSceneFrame();

        // On soumet les models et on les affiches en dessinant la stack
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu � la place
        m_Shader->Bind();
        m_Shader->SetModelMatrix(*m_DemoObject);
        m_Shader->SetUniform3f("pointLights[0].position", m_PointLightObject->GetPosition());
        m_Shader->SetUniform3f("camPos", camPos);
        m_Shader->SetProjectionViewMatrix(cam);
        //Elevate::Renderer::DrawStack(m_Shader); // WRONG // TODO redo and use
        //Elevate::Renderer::EndSceneFrame(m_Shader);
        
        // Rendering the editor grid
        // TODO modif le shader pour le mettre a nos normes de nomencalture de nom
        // TODO il faut modif le buffer (Dans Mesh) pour s'assurer que le shader recoit les infos qu'il veut
        m_GridShader->Bind();
        m_GridShader->SetProjectionViewMatrix(cam);
        m_GridObject->SetPosition({camPos.x, 0, camPos.z});

        // TODO OPTIMISE AS VERY CONSSUMING IN FPS (MAYBE FIND ALTERNATIVE TO GATHER COMPONENTS)
        // -> FOLLOW UP -> C'EST MODEL QUI A L'AIR DE RALENTIR LE TOUT
        m_Scene->RenderScene();
        m_EditorScene->RenderScene();
        //Elevate::Renderer::DrawStack(); // on va finir ce pipeline quand on aura besoin des performances
    }

    void OnUpdate() override
    {
        // TODO remove at some point cuz useless
        float mod = 2.0f * Elevate::Time::GetDeltaTime();

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

        // TODO faire des acesseurs public
        if (Elevate::Input::IsKeyPressed(EE_KEY_W))
            m_CameraObject->GetPosition() += cameraSpeed * cam.GetFrontVec();
        if (Elevate::Input::IsKeyPressed(EE_KEY_S))
            m_CameraObject->GetPosition() -= cameraSpeed * cam.GetFrontVec();
        if (Elevate::Input::IsKeyPressed(EE_KEY_D))
            m_CameraObject->GetPosition() -= cameraSpeed * cam.GetRightVec();
        if (Elevate::Input::IsKeyPressed(EE_KEY_A))
            m_CameraObject->GetPosition() += cameraSpeed * cam.GetRightVec();

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

            m_CameraObject->GetRotation().y += xoffset;
            m_CameraObject->GetRotation().x += yoffset;

            // clamp between 89 and -89 deg.
            if (m_CameraObject->GetRotation().x > 89.0f)
                m_CameraObject->GetRotation().x = 89.0f;
            if (m_CameraObject->GetRotation().x < -89.0f)
                m_CameraObject->GetRotation().x = -89.0f;

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

    // TODO PREVENT CODE REPETITION IF POSSIBLE
    void DrawTreeHierarchy(std::shared_ptr<Elevate::GameObject> object)
    {
        ImGuiBackendFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

        // TODO test if working properly
        if (!object->HasChild())
        {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        // TODO test if working properly
        if (object == m_SelectedObject)
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        if (ImGui::TreeNodeEx(object->GetName().c_str(), nodeFlags))
        {
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                SelectObject(object);
            }

            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Delete")) { /*TODO REMOVE*/ }
                if (ImGui::MenuItem("Rename")) { /*TODO RENAME*/ }

                ImGui::EndPopup();
            }

            for each (std::shared_ptr<Elevate::GameObject> child in object->GetChilds())
            {
                DrawTreeHierarchy(child);
            }
            ImGui::TreePop();
        }
    }

    void SelectObject(std::shared_ptr<Elevate::GameObject> newSelection)
    {
        // TODO : FAIRE QUE TOUS LES OBJETS EN DESSOUS SOIT AUSSI SELECTIONN� MAIS PAS AU M�ME NIVEUA
        m_SelectedObject = newSelection;
    }

    void UpdateViewportAspectRatio()
    {
        const glm::ivec2 values = aspectRatioSettings[aspectRatioValue];
        const float ratio = (float)values.x / (float)values.y;
        // Todo: faire que ce soit toutes les cam�ras qui utilisent ce ratio
        cam.UpdateAspectRatio(ratio);
    }

    std::string GetAspectRatioText(glm::ivec2 ar)
    {
        return (std::to_string(ar.x) + ":" + std::to_string(ar.y));
    }

    std::string GuizmoOperationToString(int tool)
    {
        switch (tool)
        {
        case 7: return "Translate";
        case 120: return "Rotate";
        case 896: return "Scale";
        case 14463: return "Universal";
        default: return "Unknown";
        }
    }

    void OnImGuiRender() override
    {
        ImGuiIO& io = ImGui::GetIO();

        //// TODO SEPERATE WIDGETS IN INDIVIDUAL FILES ASAP

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // TODO IMPL. FEATURES
                // TODO handle projects
                if (ImGui::MenuItem("New project")) { /* Action pour cr�er une nouvelle sc�ne */ }
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

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

        ImGui::Begin("Hierarchy");

        // TODO support pour plusieurs scenes ouvertes?????
        // TODO test if working properly
        if (!m_Scene->GetRootObjects().empty())
        {
            if (ImGui::TreeNodeEx(m_Scene->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth))
            {
                for (std::shared_ptr<Elevate::GameObject> object : m_Scene->GetRootObjects())
                {
                    DrawTreeHierarchy(object);
                }
                ImGui::TreePop();
            }
        }



        // To unselect a selected item (if selected)
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                SelectObject(nullptr);
            }
        }

        // Drag and Drop pour r�organiser
        //if (ImGui::BeginDragDropSource())
        //{
        //    ImGui::SetDragDropPayload("DND_OBJECT", &m_SelectedObject, sizeof(m_SelectedObject));
        //    ImGui::Text("Dragging %s", m_SelectedObject->GetName().c_str());
        //    ImGui::EndDragDropSource();
        //}
        //if (ImGui::BeginDragDropTarget())
        //{
        //    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_OBJECT"))
        //    {
        //        // Logique pour r�organiser les objets dans la hi�rarchie
        //    }
        //    ImGui::EndDragDropTarget();
        //}

        ImGui::End();

        ///////////////////////
        ImGui::Begin("Analyse");

        // todo add add a selected object GameObject
        if (m_SelectedObject != nullptr)
        {
            Elevate::UI::InputField("Name: ", m_SelectedObject->GetName());

            // TODO impl dans la classe Component qui est capable de s�rialiser le tout
            // PLacer dans un rendu de transform
            ImGui::SeparatorText("Transform");
            ImGui::InputFloat3("Position", glm::value_ptr(m_SelectedObject->GetPosition()));
            ImGui::InputFloat3("Rotation", glm::value_ptr(m_SelectedObject->GetRotation()));
            ImGui::InputFloat3("Scale", glm::value_ptr(m_SelectedObject->GetScale()));
            // TODO RENDERRENDER ALL THE OTHER COMPONENTS SERIALIZED
        }

        ImGui::End();

        // SCENE VIEW /////////////////
        ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_MenuBar);

        int testInt = 0;
        if (ImGui::BeginMenuBar())
        {
            // Aspect Ratio Selection
            if (ImGui::BeginMenu(GuizmoOperationToString(m_CurrentEditorTool).c_str()))
            {
                ImGui::RadioButton("Translate", &m_CurrentEditorTool, 7);
                ImGui::RadioButton("Rotate", &m_CurrentEditorTool, 120);
                ImGui::RadioButton("Scale", &m_CurrentEditorTool, 896);
                ImGui::RadioButton("Universal", &m_CurrentEditorTool, 14463);
                ImGui::EndMenu();
            }

            // Permet d'aligner le prochain menu � gauche en fonction de la taille du texte
            ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize("XXX:XXX").x - ImGui::GetCursorPos().x - 10.0f, 0.0f));

            // Aspect Ratio Selection
            if (ImGui::BeginMenu(GetAspectRatioText(aspectRatioSettings[aspectRatioValue]).c_str()))
            {
                for (int i = 0; i < sizeof(aspectRatioSettings) / sizeof(aspectRatioSettings[0]); i++)
                {
                    if (ImGui::RadioButton(GetAspectRatioText(aspectRatioSettings[i]).c_str(), &aspectRatioValue, i)) { UpdateViewportAspectRatio(); }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // we access the ImGui window size
        uint32_t window_width = (uint32_t) ImGui::GetContentRegionAvail().x;
        uint32_t window_height = (uint32_t) ImGui::GetContentRegionAvail().y;

        // Keeping the aspect ratio for the scene view
        const glm::ivec2 aspect = aspectRatioSettings[aspectRatioValue];
        const float arX = (float) aspect.x;
        const float arY = (float) aspect.y;
        const float qtX = window_width / arX;
        const float qtY = window_height / arY;
        if (qtX < qtY)
            window_height = (uint32_t) (qtX * arY);
        else
            window_width = (uint32_t) (qtY * arX);

        std::shared_ptr<Elevate::FrameBuffer> frameBuffer = Elevate::Application::Get().GetFrameBuffer();
        // we rescale the framebuffer to the actual window size here and reset the glViewport 
        frameBuffer->Rescale(window_width, window_height);
        glViewport(0, 0, window_width, window_height); // TODO remove from here asap

        // we get the screen position of the window
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::Image((void*)(intptr_t)frameBuffer->GetTextureId(), ImVec2((float)window_width, (float)window_height), ImVec2(0, 1), ImVec2(1, 0));

        // ImGuizmo //////////////////////////////////////////
        if (m_SelectedObject != nullptr)
        {
            ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
            ImGuizmo::SetOrthographic(false); // TODO SET DINAMICLY FROM THE EDITOR AND SETUP THE CAMERA ACCORDINGLY
            ImGuizmo::SetRect(pos.x, pos.y, (float)window_width, (float)window_height);

            glm::mat4 cameraProjection = cam.GetProjectionMatrix();
            glm::mat4 cameraView = cam.GenViewMatrix();
            glm::mat4 entityMatrix = m_SelectedObject->GetModelMatrix();

            // TODO SET VIA BUTTONS
            ImGuizmo::Manipulate(
                glm::value_ptr(cameraView),
                glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION) m_CurrentEditorTool,    // Change to ROTATE or SCALE as neeSded
                ImGuizmo::LOCAL,        // Change to LOCAL if needed
                glm::value_ptr(entityMatrix)
            );

            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;

            if (ImGuizmo::IsUsingAny())
            {
                ImGuizmo::DecomposeMatrixToComponents
                (
                    glm::value_ptr(entityMatrix),
                    glm::value_ptr(position),
                    glm::value_ptr(rotation),
                    glm::value_ptr(scale)
                );

                m_SelectedObject->SetScale(scale);
                m_SelectedObject->SetRotation(rotation);
                m_SelectedObject->SetPosition(position);
            }
        }

        ImGui::End();
        ////////////////////////////////////////////////////////

 
        if (ImGui::Begin("Stats"))
        {
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