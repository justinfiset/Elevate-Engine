#include <ElevateEngine.h>
#include "ElevateEngine/../../vendor/ImGui/imgui.h"

using namespace Elevate;

class LauncherLayer : public Elevate::Layer
{
public:
    static constexpr  float headerHeight = 60.0f;
    static constexpr float sidebarWidth = 300.0f;

    LauncherLayer() : Layer("Launcher") {}

    TexturePtr m_elevateIcon;

    void OnAttach() override
    {
        m_elevateIcon = Texture::CreateFromFile("./Content/Textures/Elevate.png");
    }

    void OnImGuiRender() override
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        ImGui::Begin("Fullscreen Window", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground
        );

        ImGui::BeginChild("Header", ImVec2(0, headerHeight), true);
        {
            DrawHeader();
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(0, headerHeight));
        ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, -1), true);
        {
            DrawSidebar();
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(sidebarWidth, headerHeight));
        ImGui::BeginChild("Content", ImVec2(-1, -1), true);
        {
            DrawContent();
        }
        ImGui::EndChild();

        ImGui::End();
        ImGui::PopStyleVar(3);
    }

    void DrawHeader()
    {
        float iconWidth = m_elevateIcon->GetWidth() * (headerHeight / m_elevateIcon->GetHeight());
        ImGui::Image((ImTextureID)m_elevateIcon->GetNativeHandle(), ImVec2(iconWidth, headerHeight));
    }

    void DrawSidebar()
    {

    }

    void DrawContent()
    {

    }
};

class LauncherApp : public Elevate::Application
{
public:
    LauncherApp()
    {
        PushLayer(new LauncherLayer());
    }
    ~LauncherApp() = default;
};

Elevate::Application* Elevate::CreateApplication() 
{
    return new LauncherApp();
}   