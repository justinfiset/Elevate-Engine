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
	ImGui::SameLine();
	ImGui::Checkbox("Auto-scroll", &m_autoScroll);
	ImGui::EndGroup();

	if (ImGui::BeginTable("elevate_console", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY, ImVec2(0.0f, 0.0f)))
	{
		ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(m_logs.size()));
		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
			{
				const auto msg = m_logs[i];
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImGui::Text("[%s]", msg.timeText.c_str());

				ImGui::TableNextColumn();
				ImGui::PushStyleColor(ImGuiCol_Text, msg.color);
				ImGui::TextWrapped("%s", msg.message.c_str());
				ImGui::PopStyleColor();
			}
		}

		if (m_autoScroll && ImGui::GetScrollY() > ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
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
