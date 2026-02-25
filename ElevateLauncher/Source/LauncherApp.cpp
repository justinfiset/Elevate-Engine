#include <ElevateEngine.h>
#include "ElevateEngine/../../vendor/ImGui/imgui.h"

#include <vector>

using namespace Elevate;

class LauncherLayer : public Elevate::Layer
{
public:
    static constexpr  float headerHeight = 60.0f;
    static constexpr float sidebarWidth = 300.0f;

    static constexpr std::array<const char*, 2> projectOptions = {
        "New Project",
        "Open Project",
    };

    static constexpr std::array<const char*, 1> learningOptions = {
        "Documentation",
    };

    static constexpr std::array<const char*, 2> communityOptions = {
        "Report an Issue",
        "Rate the Engine"
    };

    std::shared_ptr<Texture> m_elevateIcon = nullptr;

    LauncherLayer() : Layer("Launcher") {}

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

        ImGui::Begin("Fullscreen Window", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoNavFocus
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
        ImGui::PopStyleVar();
    }

    void DrawHeader()
    {
        float iconWidth = m_elevateIcon->GetWidth() * (headerHeight / m_elevateIcon->GetHeight());
        ImGui::Image((ImTextureID)m_elevateIcon->GetNativeHandle(), ImVec2(iconWidth, headerHeight));
    }

    void DrawSidebar()
    {
        static float buttonHeight = 40.0f;

        ImGui::SeparatorText("Projects");
        for (const char* option : projectOptions)
        {
            if (ImGui::Button(option, ImVec2(sidebarWidth, buttonHeight)))
            {
                // Button action
            }
        }

        ImGui::Dummy(ImVec2(sidebarWidth, buttonHeight / 4));

        ImGui::SeparatorText("Learn");
        for (const char* option : learningOptions)
        {
            if (ImGui::Button(option, ImVec2(sidebarWidth, buttonHeight)))
            {
                // Button action
            }
        }

        ImGui::SeparatorText("Community");
        for (const char* option : communityOptions)
        {
            if (ImGui::Button(option, ImVec2(sidebarWidth, buttonHeight)))
            {
                // Button action
            }
        }
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