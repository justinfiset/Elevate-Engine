module;

#include <string>
#include <deque>

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
	class ConsolePanel : public Widget
	{
	public:
		ConsolePanel();
		virtual ~ConsolePanel();

		void OnImGuiRender() override;
		void LogCallback(LogLevel level, std::string_view text);

	private:
		std::deque<EditorMessage> m_logs;

		CallbackID m_coreLoggerCallback;
		CallbackID m_loggerCallback;
	};
}