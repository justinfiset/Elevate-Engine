#include <ElevateEngine.h>
#include "ElevateEngine/../../vendor/ImGui/imgui.h"

#include <span>
#include <vector>

// Elevate Launcher includes
#include "Managers/ProjectManager.h"
#include "Controllers/ProjectController.h"

using namespace EL;

using ButtonAction = std::function<void()>;
using LauncherButton = std::pair<std::string, ButtonAction>;

class LauncherLayer : public Elevate::Layer
{
private:
	ProjectController& m_controller;

	static constexpr  float headerHeight = 60.0f;
	static constexpr float sidebarWidth = 300.0f;

	std::shared_ptr<Elevate::Texture> m_elevateIcon = nullptr;
	std::array<LauncherButton, 2> m_projectOptions = {{
		{ "New Project", [this]() { m_controller.SetActiveTab(LauncherTab::ProjectCreation); } },
		{ "Open Project", [this]() { m_controller.SetActiveTab(LauncherTab::ProjectList); } }
	}};

	std::array<LauncherButton, 1> m_learningOptions = {{
		{ "Documentation", [this]() { m_controller.OpenDocumentation(); } }
	}};

	std::array<LauncherButton, 2> m_communityOptions = {{
		{ "Report an Issue", [this]() { m_controller.ReportIssue(); } },
		{ "Rate the Engine", [this]() { m_controller.RateEngine(); } }
	}};

public:
	LauncherLayer(ProjectController& controller) : Layer("Launcher"), m_controller(controller) { }

	void OnAttach() override
	{
		m_elevateIcon = Elevate::Texture::CreateFromFile("Content/Textures/Elevate.png");
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

	void DrawButtonList(std::span<const LauncherButton> buttons, float buttonHeight)
	{
		for (LauncherButton button : buttons)
		{
			if (ImGui::Button(button.first.c_str(), ImVec2(sidebarWidth, buttonHeight)))
			{
				button.second();
			}
		}
	}

	void DrawSidebar()
	{
		static float buttonHeight = 40.0f;

		ImGui::SeparatorText("Projects");
		DrawButtonList(m_projectOptions, buttonHeight);

		ImGui::Dummy(ImVec2(sidebarWidth, buttonHeight / 4));
		ImGui::SeparatorText("Learn");
		DrawButtonList(m_learningOptions, buttonHeight);

		ImGui::SeparatorText("Community");
		DrawButtonList(m_communityOptions, buttonHeight);
	}

	// Content panel drawing
	void DrawProjectCreationMenu() { }
	void DrawProjectList() { }
	void DrawSettings() { }

	void DrawContent()
	{
		switch (m_controller.GetActiveTab())
		{
		case LauncherTab::ProjectCreation:		DrawProjectCreationMenu(); break;
		case LauncherTab::ProjectList:			DrawProjectList(); break;
		case LauncherTab::Settings:				DrawSettings(); break;
		default: return;
		}
	}
};

class LauncherApp : public Elevate::Application
{
public:
	LauncherApp() : m_controller(m_manager)
	{
		PushLayer(new LauncherLayer(m_controller));
	}
	~LauncherApp() = default;

private:
	ProjectManager m_manager;
	ProjectController m_controller;
};

Elevate::Application* Elevate::CreateApplication() 
{
	return new LauncherApp();
}