module;

#include <format>
#include <chrono>

#include <imgui.h>
#include <glm/vec4.hpp>

module Elevate.Editor.Panels.ConsolePanel;

import Elevate.Foundations.CoreLogger;

Elevate::Editor::ConsolePanel::ConsolePanel()
{
	m_coreLoggerCallback = CoreLogger::AddCallback([this](LogLevel level, std::string_view text)
		{
			LogCallback(level, text);
		});
	m_loggerCallback = Logger::AddCallback([this](LogLevel level, std::string_view text)
		{
			LogCallback(level, text);
		});
}

Elevate::Editor::ConsolePanel::~ConsolePanel()
{
	CoreLogger::RemoveCallback(m_coreLoggerCallback);
	Logger::RemoveCallback(m_loggerCallback);
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
	constexpr size_t MAX_LOGS = 1000;
	EditorMessage::Type type;

	switch (level)
	{
	case LogLevel::Trace:
		type = EditorMessage::Type::Message;
		break;
	case LogLevel::Info:
		type = EditorMessage::Type::Information;
		break;
	case LogLevel::Warning:
		type = EditorMessage::Type::Warning;
		break;
	case LogLevel::Error:
		type = EditorMessage::Type::Error;
		break;
	case LogLevel::Fatal:
		type = EditorMessage::Type::Error;
		break;
	default:
		return;
	}

	m_logs.emplace_back(std::string(text), type);

	if (m_logs.size() > MAX_LOGS)
	{
		m_logs.pop_front();
	}
}
