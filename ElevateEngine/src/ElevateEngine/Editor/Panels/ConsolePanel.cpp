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

	for (const auto& msg : m_logs)
	{
		glm::vec4 color = msg.GetColor();
		ImGui::TextColored(ImVec4(color.r, color.g, color.b, color.a), msg.message.c_str());
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
