#pragma once

#include <string>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Editor/UI/EditorMessage.h>

struct ImVec4;

namespace Elevate::Editor
{
	class ConsolePanel : public EditorWidget
	{
	public:
		ConsolePanel();
		void OnImGuiRender() override;
		void LogCallback(LogLevel level, std::string_view text);

	private:
		std::vector<EditorMessage> m_logs;
	};
}