#include "eepch.h"
#include "EditorLayer.h"

#include <ElevateEngine/Renderer/Model.h>
#include <ElevateEngine/Scene/SceneManager.h>

#include "imgui.h"

#include "ElevateEngine/Renderer/Shader/ShaderManager.h"
#include "ElevateEngine/Renderer/Model.h"
#include <ElevateEngine/Editor/Widgets/SkyboxEditorWidget.h>
namespace Elevate::Editor
{
    EditorLayer* EditorLayer::s_Instance = nullptr;

    EditorLayer::EditorLayer()
    {
        s_Instance = this;

        InitUI();
    }

    void EditorLayer::InitUI()
    {
        m_ScenePanel = std::make_unique<ScenePanel>();
        m_HierarchyPanel = std::make_unique<HierarchyPanel>();
        m_AnalyserPanel = std::make_unique<AnalyserPanel>();
        m_StatisticsPanel = std::make_unique<StatisticsPanel>();
        m_AssetBrowserPanel = std::make_unique<AssetBrowserPanel>();
    }

    void EditorLayer::OnAttach()
    {
        m_EditorScene = Scene::Create("Editor", SceneType::EditorScene);

        // Editor Camera
        m_CameraObject = GameObject::Create("Editor Camera", m_EditorScene);
        m_CameraObject->AddComponent<EditorCamera>(60.0f);

        // Setup the grid shader ///////////////////////////
        m_GridShader = Shader::CreateFromFiles(
            "shader/grid.vert",
            "shader/grid.frag"
        );
        m_GridShader->Bind();
        m_GridShader->SetUniform4f("lineColor", { 0.9, 0.9, 0.9, 0.5 });
        m_GridShader->SetUniform4f("backgroundColor", { 0.6, 0.6, 0.6, 0.025 });
        //////////////////////////////////////////////

        // Grid
        m_GridObject = GameObject::Create("Editor Grid", m_EditorScene);
        Model& gridModel = m_GridObject->AddComponent<Model>("model/plane.obj");
        gridModel.SetShader(m_GridShader);
        m_GridObject->SetScale({ 50, 50, 50 });

        // TODO SET AUTOMATICLY SOMEWHERE
        SceneManager::LoadScene(m_EditorScene);

        CreateWidget<SkyboxEditorWidget>();
    }

    void EditorLayer::OnUpdate()
    {
        m_AssetBrowserPanel->OnUpdate();
        m_EditorScene->UpdateScene();
    }

    void EditorLayer::OnRender()
    {
        // Rendering the editor grid
        // TODO modif le shader pour le mettre a nos normes de nomencalture de nom
        // TODO il faut modif le buffer (Dans Mesh) pour s'assurer que le shader recoit les infos qu'il veut et non top d'info inutiles
        glm::vec3 camPos = m_CameraObject->GetPosition();
        m_GridShader->Bind();
        m_GridShader->SetProjectionViewMatrix(*GetCamera());
        m_GridObject->SetPosition({ camPos.x, 0, camPos.z });

        m_EditorScene->RenderScene(); // TODO AUTOMATISER
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGuiIO& io = ImGui::GetIO();

        // MENU
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
                if (ImGui::MenuItem("Cube")) {
                    ScenePtr scene = SceneManager::GetCurrentScene(SceneType::RuntimeScene);
                    Elevate::GameObjectPtr m_DemoCube = Elevate::GameObject::Create("Cube", scene);
                    m_DemoCube->AddComponent<Elevate::Model>("model/cube.obj");
                }
                if (ImGui::MenuItem("Plane")) {
                    ScenePtr scene = SceneManager::GetCurrentScene(SceneType::RuntimeScene);
                    Elevate::GameObjectPtr m_DemoCube = Elevate::GameObject::Create("Plane", scene);
                    m_DemoCube->AddComponent<Elevate::Model>("model/plane.obj");
                }
                // TODO ADD OTHER MODELS
                //if (ImGui::MenuItem("Sphere")) {

                //}
                //if (ImGui::MenuItem("Ico Sphere")) {

                //}
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

        // TODO add in the widget stack
        m_ScenePanel->OnImGuiRender();
        m_HierarchyPanel->OnImGuiRender();
        m_AnalyserPanel->OnImGuiRender();
        m_StatisticsPanel->OnImGuiRender();
        m_AssetBrowserPanel->OnImGuiRender();

        for (auto& widgetPtr : m_widgets)
            widgetPtr->OnImGuiRender();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        if (event.GetEventType() == EventType::MouseMoved)
        {
            //MouseMovedEvent& mouseEvent = dynamic_cast<MouseMovedEvent&>(event);
        }

        m_EditorScene->Notify(event);
    }

    void EditorLayer::SelectObject(GameObjectPtr newSelection)
    {
        // TODO : FAIRE QUE TOUS LES OBJETS EN DESSOUS SOIT AUSSI SELECTIONN� MAIS PAS AU M�ME NIVEUA
        m_SelectedObject = newSelection;
    }
}