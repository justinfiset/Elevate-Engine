#include "StatisticsPanel.h"

#include <imgui.h>
#include "ConsolePanel.h"

Elevate::Editor::ConsolePanel::ConsolePanel()
{
	Elevate::Log::GetCoreLogger()->AddCallback([this](LogLevel level, std::string_view text) {
		LogCallback(level, text);
	});
	Elevate::Log::GetClientLogger()->AddCallback([this](LogLevel level, std::string_view text) {
		LogCallback(level, text);
	});
}

void Elevate::Editor::ConsolePanel::OnImGuiRender()
{
	ImGui::Begin("Console");

	ImGui::BeginGroup();
	if (ImGui::Button("Clear"))
	{
		m_logs.clear();
	}
	ImGui::EndGroup();

	if (ImGui::BeginTable("elevate_console", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY, ImVec2(0.0f, 0.0f)))
	{
		ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		for (const auto& msg : m_logs)
		{
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			std::string time = std::format("{:%H:%M:%S}", msg.timestamp);
			ImGui::Text("[%s]", time.c_str());

			ImGui::TableNextColumn();
			glm::vec4 color = msg.GetColor();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.r, color.g, color.b, color.a));
			ImGui::TextWrapped("%s", msg.message.c_str());
			ImGui::PopStyleColor();
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void Elevate::Editor::ConsolePanel::LogCallback(LogLevel level, std::string_view text)
{
	switch (level)
	{
	case LogLevel::Trace:
		m_logs.push_back(EditorMessage(std::string(text), EditorMessage::Type::Message));
		break;
	case LogLevel::Info:
		m_logs.push_back(EditorMessage(std::string(text), EditorMessage::Type::Information));
		break;
	case LogLevel::Warning:
		m_logs.push_back(EditorMessage(std::string(text), EditorMessage::Type::Warning));
		break;
	case LogLevel::Error:
		m_logs.push_back(EditorMessage(std::string(text), EditorMessage::Type::Error));
		break;
	case LogLevel::Fatal:
		m_logs.push_back(EditorMessage(std::string(text), EditorMessage::Type::Error));
		break;
	}
}
