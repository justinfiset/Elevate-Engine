#include <ElevateEngine.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <span>
#include <vector>

// Elevate Launcher includes
#include "Managers/ProjectManager.h"
#include "Controllers/ProjectController.h"

using namespace EL;

using ButtonAction = std::function<void()>;
using LauncherButton = std::pair<std::string, ButtonAction>;

namespace Color
{
	inline const ImVec4 Green = { 0.098f, 0.529f, 0.329f, 1.0f };
	inline const ImVec4 Orange = { 1.000f, 0.757f, 0.027f, 1.0f };
	inline const ImVec4 Red = { 0.863f, 0.208f, 0.271f, 1.0f };
	inline const ImVec4 Accent = { 0.2f, 0.4f, 0.8f, 1.0f };
}

class LauncherLayer : public Elevate::Layer
{
private:
	ProjectController& m_controller;

	static constexpr  float headerHeight = 60.0f;

	static constexpr float padding = 10.0f;
	static constexpr float sidebarWidth = 300.0f;
	static constexpr float sidebarButtonHeight = 40.0f;
	static constexpr float closeButtonSize = 20.0f;

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

		ImGui::SetCursorPos(ImVec2(sidebarWidth + padding, headerHeight + padding));
		ImGui::BeginChild("Content", ImVec2(-1, -1), false);
		{
			DrawNotifications();
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

	void DrawButtonList(std::span<const LauncherButton> buttons, float buttonWidth, float buttonHeight)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		for (LauncherButton button : buttons)
		{
			if (ImGui::Button(button.first.c_str(), ImVec2(buttonWidth, buttonHeight)))
			{
				button.second();
			}
		}
		ImGui::PopStyleVar();
	}

	void DrawSidebar()
	{
		ImGui::SeparatorText("Projects");
		DrawButtonList(m_projectOptions, sidebarWidth, sidebarButtonHeight);
		ImGui::Dummy(ImVec2(sidebarWidth, padding));

		ImGui::SeparatorText("Learn");
		DrawButtonList(m_learningOptions, sidebarWidth, sidebarButtonHeight);

		ImGui::SeparatorText("Community");
		DrawButtonList(m_communityOptions, sidebarWidth, sidebarButtonHeight);
	}

	void SetButtonColor(ImVec4 baseColor)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, baseColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(baseColor.x + 0.1f, baseColor.y + 0.1f, baseColor.z + 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(baseColor.x - 0.1f, baseColor.y - 0.1f, baseColor.z - 0.1f, 1.0f));
	}

	void PopButtonColor()
	{
		ImGui::PopStyleColor(3);
	}

	// Content panel drawing
	void DrawProjectCreationMenu()
	{
		static ProjectCreationProps props;
		LauncherButton cancelButton = { "Cancel", [this]() { m_controller.SetActiveTab(LauncherTab::ProjectList); } };
		LauncherButton createButton = { "Create", [this]() { m_controller.CreateNewProject(props); } };

		ImGui::SeparatorText("Create a New Project");
		ImGui::NewLine();

		// Project form
		ImGui::Text("Project Name");
		ImGui::InputText("##ProjectName", &props.Name);
		ImGui::NewLine();
		ImGui::Text("Project Path");
		ImGui::InputText("##ProjectPath", &props.Path);

		// Buttons at the bottom
		ImGui::NewLine();
		SetButtonColor(Color::Red);
		DrawButtonList(std::span(&cancelButton, 1), sidebarWidth, sidebarButtonHeight);
		PopButtonColor();

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(padding, sidebarButtonHeight));
		ImGui::SameLine();

		SetButtonColor(Color::Accent);
		DrawButtonList(std::span(&createButton, 1), sidebarWidth, sidebarButtonHeight);
		PopButtonColor();
	}

	void DrawProjectList() { }
	void DrawSettings() { }
	
	void DrawNotifications()
	{
		auto& notifications = m_controller.GetNotifications();

		for (const auto& notification : notifications)
		{
			ImVec2 pos = ImGui::GetCursorScreenPos();
			std::string text = "- " + notification.Message;
			
			ImVec4 color;
			switch (notification.Type)
			{
			case Notification::MsgType::Success:
				color = Color::Green;
				break;
			case Notification::MsgType::Info:
				color = Color::Accent;
				break;
			case Notification::MsgType::Warning:
				color = Color::Orange;
				break;
			case Notification::MsgType::Error:
				color = Color::Red;
				break;
			}

			float maxWidth = ImGui::GetContentRegionAvail().x - padding;
			float maxTextWidth = maxWidth - closeButtonSize - padding;
			ImVec2 size = ImGui::CalcTextSize(text.c_str(), nullptr, false, maxTextWidth);
			ImGui::PushID(notification.Id);
			float rectHeight = size.y + 2 * padding;
			ImGui::GetWindowDrawList()->AddRectFilled(
				pos,
				ImVec2(pos.x + maxWidth, pos.y + rectHeight),
				ImGui::GetColorU32(color),
				5.0f
			);
			
			LauncherButton closeBtn = { "x", [this, notification]() { m_controller.RemoveNotification(notification.Id); } };

			ImGui::SetCursorScreenPos(ImVec2(pos.x + maxWidth - closeButtonSize - padding, pos.y + (size.y) / 2));
			DrawButtonList(std::span(&closeBtn, 1), closeButtonSize, closeButtonSize);
			
			ImGui::SetCursorScreenPos(ImVec2(pos.x + padding, pos.y + padding));
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxWidth - padding * 4 - closeButtonSize);
			ImGui::TextUnformatted(text.c_str());
			ImGui::PopTextWrapPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + rectHeight + padding));
			ImGui::PopID();
		}
	}

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