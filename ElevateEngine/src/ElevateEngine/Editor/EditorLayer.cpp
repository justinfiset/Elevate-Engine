#include "eepch.h"
#include "EditorLayer.h"

#include <ElevateEngine/Renderer/Model.h>
#include <ElevateEngine/Scene/SceneManager.h>

#include "imgui.h"
#include "ElevateEngine/Renderer/Shader/ShaderManager.h"
#include "ElevateEngine/Renderer/Model.h"

#include <ElevateEngine/Editor/Camera/EditorCamera.h>

#include <ElevateEngine/Editor/Panels/ScenePanel.h>
#include <ElevateEngine/Editor/Panels/HierarchyPanel.h>
#include <ElevateEngine/Editor/Panels/AnalyserPanel.h>
#include <ElevateEngine/Editor/Panels/AssetBrowserPanel.h>
#include <ElevateEngine/Editor/Panels/StatisticsPanel.h>

#include <ElevateEngine/Editor/Widgets/SkyboxEditorWidget.h>
#include <ElevateEngine/ImGui/ImGuiTheme.h>

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
        CreateWidget<HierarchyPanel>();
        CreateWidget<ScenePanel>();
        CreateWidget<AnalyserPanel>();
        CreateWidget<StatisticsPanel>();
        CreateWidget<AssetBrowserPanel>();
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

        CreateWidget<SkyboxEditorWidget>();
    }

    void EditorLayer::OnUpdate()
    {
        for (auto& widgetPtr : m_widgets)
            widgetPtr->OnUpdate();

        m_EditorScene->UpdateScene();
    }

    void EditorLayer::OnRender()
    {
        // Rendering the editor grid
        glm::vec3 camPos = m_CameraObject->GetPosition();
        m_GridShader->Bind();
        m_GridShader->SetProjectionViewMatrix(*GetCamera());
        m_GridObject->SetPosition({ camPos.x, 0, camPos.z });

        m_EditorScene->RenderScene(GetCamera());
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

            if (ImGui::BeginMenu("Preferences"))
            {
                if (ImGui::BeginMenu("Theme"))
                {
                    if (ImGui::MenuItem("Dark", NULL, UI::GetCurrentTheme() == UI::UITheme::Dark))
                    {
                        UI::SetDarkTheme();
                    }
                    if (ImGui::MenuItem("Light", NULL, UI::GetCurrentTheme() == UI::UITheme::Light))
                    {
                        UI::SetLightTheme();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        float toolbarHeight = 50.0f;

        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + toolbarHeight + ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - ImGui::GetFrameHeight() - toolbarHeight));
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Prevent a weird black stripe arround the dockspace
        ImGui::Begin("DockSpaceWindow", nullptr, window_flags);

        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarHeight));
        ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking;

        ImGui::Begin("Toolbar", nullptr, toolbarFlags);

        TexturePtr playTexture = Texture::Create("./editor/icons/light/play.png");
        if (ImGui::ImageButton("play", (ImTextureID)(intptr_t)playTexture->GetID(), ImVec2(32, 32)))
        {

        }
        ImGui::SameLine();
        TexturePtr pauseTexture = Texture::Create("./editor/icons/light/pause.png");
        if (ImGui::ImageButton("pause", (ImTextureID)(intptr_t)pauseTexture->GetID(), ImVec2(32, 32)))
        {

        }
        ImGui::End();

        for (auto& widgetPtr : m_widgets)
            widgetPtr->OnImGuiRender();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_EditorScene->Notify(event);
    }

    void EditorLayer::SelectObject(GameObjectPtr newSelection)
    {
        // TODO : FAIRE QUE TOUS LES OBJETS EN DESSOUS SOIT AUSSI SELECTIONN� MAIS PAS AU M�ME NIVEUA
        m_SelectedObject = newSelection;
    }
}