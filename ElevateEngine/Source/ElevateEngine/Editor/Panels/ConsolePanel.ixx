module;

#include <string>
#include <deque>
#include <mutex>

#include <imgui.h>

export module Elevate.Editor.Panels.ConsolePanel;

// Foundations
import Elevate.Foundations.Logger;
import Elevate.Foundations.Callbacks;
// Editor
import Elevate.Editor.Widgets.Widget;
import Elevate.Editor.Message;

export namespace Elevate::Editor
{
	struct ConsoleMessage : EditorMessage
	{
		std::string timeText;	// Contains the timestamp already formatted
		ImVec4 color;			// Resolve the color once

		ConsoleMessage(std::string message, Type type);
	};

	class ConsolePanel : public Widget
	{
	public:
		ConsolePanel();
		virtual ~ConsolePanel();

		void OnImGuiRender() override;
		void LogCallback(LogLevel level, std::string_view text);

	private:
		std::deque<ConsoleMessage> m_logs;

		CallbackID m_coreLoggerCallback;
		CallbackID m_loggerCallback;

		bool m_autoScroll;
	};
}