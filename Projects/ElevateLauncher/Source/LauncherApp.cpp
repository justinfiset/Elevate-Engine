#include <ElevateEngine.h>
#include <ElevateEngine/Core/PathResolver.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <span>
#include <vector>

// Elevate Launcher includes
#include "Managers/ProjectManager.h"
#include "Controllers/ProjectController.h"
#include "Models/Project.h"

using namespace EL;

using ButtonAction = std::function<void()>;
using LauncherButton = std::pair<std::string, ButtonAction>;

namespace Color
{
	inline const ImVec4 Green = { 0.098f, 0.529f, 0.329f, 1.0f };
	inline const ImVec4 Orange = { 1.000f, 0.757f, 0.027f, 1.0f };
	inline const ImVec4 Red = { 0.863f, 0.208f, 0.271f, 1.0f };
	inline const ImVec4 Accent = { 0.2f, 0.4f, 0.8f, 1.0f };
	inline const ImVec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
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
	std::shared_ptr<Elevate::Texture> m_createTexture = nullptr;
	std::shared_ptr<Elevate::Texture> m_voidTexture = nullptr;

	ImFont* m_defaultFont = nullptr;
	ImFont* m_headerFont = nullptr;
	ImFont* m_subHeaderFont = nullptr;
	ImFont* m_bigFont = nullptr;

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
		m_elevateIcon = Elevate::Texture::CreateFromFile("content://Textures/Elevate.png");
		m_createTexture = Elevate::Texture::CreateFromFile("content://Textures/add-files.png");
		m_voidTexture = Elevate::Texture::CreateFromFile("content://Textures/void.png");

		ImGuiIO& io = ImGui::GetIO();
		m_defaultFont = io.Fonts->AddFontFromFileTTF(Elevate::PathResolver::Resolve("content://Fonts/OpenSans_SemiCondensed-SemiBold.ttf").c_str(), 18.0f);
		m_headerFont = io.Fonts->AddFontFromFileTTF(Elevate::PathResolver::Resolve("content://Fonts/OpenSans_SemiCondensed-SemiBold.ttf").c_str(), 28.0f);
		m_subHeaderFont = io.Fonts->AddFontFromFileTTF(Elevate::PathResolver::Resolve("content://Fonts/OpenSans_SemiCondensed-SemiBold.ttf").c_str(), 23.0f);
		m_bigFont = io.Fonts->AddFontFromFileTTF(Elevate::PathResolver::Resolve("content://Fonts/OpenSans_SemiCondensed-SemiBold.ttf").c_str(), 40.0f);
	}

	void OnImGuiRender() override
	{
		ImGui::PushFont(m_defaultFont);

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
		ImGui::PopFont();
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
		ImVec4 borderColor = ImVec4(
			std::min(baseColor.x + 0.15f, 1.0f),
			std::min(baseColor.y + 0.15f, 1.0f),
			std::min(baseColor.z + 0.15f, 1.0f),
			1.0f
		);

		ImVec4 hoverColor = ImVec4(baseColor.x + 0.08f, baseColor.y + 0.08f, baseColor.z + 0.08f, 1.0f);
		ImVec4 activeColor = ImVec4(baseColor.x - 0.1f, baseColor.y - 0.1f, baseColor.z - 0.1f, 1.0f);

		ImGui::PushStyleColor(ImGuiCol_Button, baseColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);

		ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.2f);
	}

	void PopButtonColor()
	{
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(1);
	}

	void SetButtonColorOutlined(ImVec4 baseColor)
	{
		ImVec4 bg = baseColor; bg.w = 0.1f;
		ImVec4 hover = baseColor; hover.w = 0.25f;
		ImVec4 active = baseColor; active.w = 0.4f;

		ImGui::PushStyleColor(ImGuiCol_Button, bg);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);

		ImGui::PushStyleColor(ImGuiCol_Border, baseColor);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	}

	void PopButtonColorOutlined()
	{
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(1);
	}

	void PushInputStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, Color::Accent);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding, padding));
	}

	void PopInputStyle()
	{
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(3);
	}

	// Content panel drawing
	void DrawProjectCreationMenu()
	{
		static float constexpr footerHeight = sidebarButtonHeight + (padding * 2);
		static ProjectCreationProps props;
		static int selectedTemplate = 0;
		auto templates = m_controller.GetProjectTemplates();

		LauncherButton cancelButton = { "Cancel", [this]() { m_controller.SetActiveTab(LauncherTab::ProjectList); } };
		LauncherButton createButton = { "Create", [this]() {
			if (m_controller.CreateNewProject(props)) {
				props = ProjectCreationProps();
			}
		} };

		ImGui::PushFont(m_headerFont);
		ImGui::Text("Create a New Project");
		ImGui::Separator();
		ImGui::PopFont();

		PushInputStyle();
		ImGuiTableFlags flags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersInnerV;
		if (ImGui::BeginTable("CreationLayout", 2, flags))
		{
			ImGui::TableSetupColumn("Project Informations", ImGuiTableColumnFlags_WidthStretch, 0.6f);
			ImGui::TableSetupColumn("Template", ImGuiTableColumnFlags_WidthStretch, 0.4f);
			ImGui::TableNextRow();

			// Project Configuration
			ImGui::TableSetColumnIndex(0);
			ImGui::NewLine();
			// Project form
			ImGui::Text("Project Name");
			ImGui::InputText("##ProjectName", &props.Name);
			ImGui::NewLine();
			ImGui::Text("Project Path");
			ImGui::InputText("##ProjectPath", &props.Path);

			const char* templateLabel = templates[selectedTemplate].Name.c_str();
			ImGui::NewLine();
			ImGui::Text("Template");
			if (ImGui::BeginCombo("##templateSelection", templateLabel))
			{
				for (int i = 0; i < templates.size(); i++)
				{
					const bool isSelected = (selectedTemplate == i);
					if (ImGui::Selectable(templates[i].Name.c_str(), isSelected))
					{
						selectedTemplate = i;
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Dummy(ImVec2(ImGui::GetColumnWidth(), ImGui::GetContentRegionAvail().y - footerHeight - padding));

			// Template Preview
			ImGui::TableSetColumnIndex(1);
			ImGui::PushFont(m_subHeaderFont);
			ImGui::Text(templates[selectedTemplate].Name.c_str());
			ImGui::Separator();
			ImGui::PopFont();

			float availWidth = ImGui::GetContentRegionAvail().x - padding;
			float textureSize = (availWidth) / 3;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			if (templates[selectedTemplate].Thumbnail)
			{
				ImVec2 p_min = ImVec2(pos.x + (availWidth - textureSize) / 2.0f, pos.y + padding);
				ImVec2 p_max = ImVec2(p_min.x + textureSize, p_min.y + textureSize);
				ImGui::GetWindowDrawList()->AddImageRounded(
					(ImTextureID)templates[selectedTemplate].Thumbnail->GetNativeHandle(),
					p_min,
					p_max,
					ImVec2(0, 0), ImVec2(1, 1),
					IM_COL32_WHITE,
					5.0f
				);
				ImGui::GetWindowDrawList()->AddRect(
					p_min,
					p_max,
					ImGui::GetColorU32(ImGuiCol_Border),
					5.0f,
					0,
					1.5f
				);
				ImGui::SetCursorScreenPos(ImVec2(pos.x + padding, p_max.y + padding));
			}
			ImGui::SeparatorText("Template Description");
			ImGui::TextWrapped(templates[selectedTemplate].Desription.c_str());

			ImGui::EndTable();
		}

		// Buttons at the bottom
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footerHeight - padding);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footerHeight - padding);

		ImGui::Dummy(ImVec2(sidebarWidth, padding));
		SetButtonColorOutlined(Color::Red);
		DrawButtonList(std::span(&cancelButton, 1), sidebarWidth, sidebarButtonHeight);
		PopButtonColorOutlined();

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(padding, sidebarButtonHeight));
		ImGui::SameLine();

		SetButtonColor(Color::Green);
		DrawButtonList(std::span(&createButton, 1), sidebarWidth, sidebarButtonHeight);
		PopButtonColor();
		ImGui::Dummy(ImVec2(sidebarWidth, padding));

		PopInputStyle();
	}

	void DrawProject(const Project& project)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		float height = 50.0f;
		float maxWidth = ImGui::GetContentRegionAvail().x - padding;
		float maxTextWidth = maxWidth - closeButtonSize - padding;
		ImVec4 textColor = project.IsValid ? Color::White : Color::Orange;

		ImGui::PushID(std::string("project" + project.Id).c_str());
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + maxWidth, pos.y + height), ImGui::GetColorU32(ImGuiCol_FrameBg), 5.0f);
		ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + maxWidth, pos.y + height), ImGui::GetColorU32(ImGuiCol_Border), 5.0f);

		float optionButtonSize = 30.0f;
		ImGui::SetCursorScreenPos(ImVec2(pos.x + maxWidth - optionButtonSize - padding, pos.y + (height - optionButtonSize) / 2));
		ImVec2 optionsButtonPos = ImGui::GetCursorScreenPos();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		if (ImGui::Button("...", ImVec2(optionButtonSize, optionButtonSize)))
		{
			ImGui::OpenPopup("ProjectContextMenu");
		}
		ImGui::PopStyleVar();
		ImGui::GetWindowDrawList()->AddRect(optionsButtonPos, ImVec2(optionsButtonPos.x + optionButtonSize, optionsButtonPos.y + optionButtonSize), ImGui::GetColorU32(ImGuiCol_Border), 5.0f);

		if (ImGui::BeginPopupContextItem("ProjectContextMenu"))
		{
			if (ImGui::MenuItem("Remove From List"))
			{
				m_controller.RemoveProjectFromList(project.Id);
			}
			ImGui::EndPopup();
		}

		ImGui::SetCursorScreenPos(ImVec2(pos.x + padding, pos.y + padding));
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxWidth - padding * 4 - optionButtonSize);
		ImGui::TextColored(textColor, project.Name.c_str());
		ImGui::SetCursorScreenPos(ImVec2(pos.x + padding, ImGui::GetCursorScreenPos().y));
		ImGui::TextDisabled(project.Path.c_str());
		ImGui::PopTextWrapPos();
		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + height + padding));
		ImGui::PopID();
	}

	void DrawProjectList()
	{
		ImGui::PushFont(m_headerFont);
		ImGui::Text("Open a Project");
		ImGui::Separator();
		ImGui::PopFont();
		ImGui::NewLine();

		const auto& projects = m_controller.GetProjectList();
		if (projects.empty())
		{
			float availWidth = ImGui::GetContentRegionAvail().x - padding;
			float textureSize = (availWidth) / 3;
			ImVec2 pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x + (availWidth - textureSize) / 2.0f, pos.y + padding));
			ImGui::Image((ImTextureID)m_voidTexture->GetNativeHandle(), ImVec2(textureSize, textureSize));
			
			const char* emptyListText = "No project yet.";
			ImGui::PushFont(m_bigFont);
			ImVec2 textSize = ImGui::CalcTextSize(emptyListText);
			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x + (availWidth - textSize.x) / 2.0f, pos.y + padding));
			ImGui::Text(emptyListText);
			ImGui::GetIO().FontGlobalScale = 1.0f;
			ImGui::PopFont();

			const char* emptyListInstruction = "Start by creating a new project...";
			pos = ImGui::GetCursorPos();
			textSize = ImGui::CalcTextSize(emptyListInstruction);
			ImGui::SetCursorPos(ImVec2(pos.x + (availWidth - textSize.x) / 2.0f, pos.y + padding));
			ImGui::TextDisabled(emptyListInstruction);

			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x + (availWidth - sidebarWidth) / 2.0f, pos.y + padding));
			SetButtonColor(Color::Accent);
			LauncherButton cancelButton = { "Create Project", [this]() { m_controller.SetActiveTab(LauncherTab::ProjectCreation); } };
			DrawButtonList(std::span(&cancelButton, 1), sidebarWidth, sidebarButtonHeight);
			PopButtonColor();
		}
		else
		{
			for (const auto& project : projects)
			{
				DrawProject(project);
			}
		}
	}

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
			ImVec4 backgroundColor = color;
			backgroundColor.w = 0.15f;

			float maxWidth = ImGui::GetContentRegionAvail().x - padding;
			float maxTextWidth = maxWidth - closeButtonSize - padding;
			ImVec2 size = ImGui::CalcTextSize(text.c_str(), nullptr, false, maxTextWidth);
			ImGui::PushID(notification.Id);
			float rectHeight = size.y + 2 * padding;
			ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + maxWidth, pos.y + rectHeight), ImGui::GetColorU32(backgroundColor), 5.0f);
			ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + maxWidth, pos.y + rectHeight), ImGui::GetColorU32(color), 5.0f);
			
			// Draw the close button
			ImGui::SetCursorScreenPos(ImVec2(pos.x + maxWidth - closeButtonSize - padding, pos.y + (rectHeight - closeButtonSize) / 2.0f));
			ImVec2 closeButtonPos = ImGui::GetCursorScreenPos();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
			if (ImGui::Button("x", ImVec2(closeButtonSize, closeButtonSize)))
			{
				m_controller.RemoveNotification(notification.Id);
			}
			ImGui::PopStyleVar();
			ImGui::GetWindowDrawList()->AddRect(closeButtonPos, ImVec2(closeButtonPos.x + closeButtonSize, closeButtonPos.y + closeButtonSize), ImGui::GetColorU32(ImGuiCol_Border), 5.0f);

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