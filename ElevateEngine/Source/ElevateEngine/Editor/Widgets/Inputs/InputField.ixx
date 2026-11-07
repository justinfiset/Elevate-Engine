module;

#include <string>
#include <functional>

export module Elevate.Editor.Widgets.InputField;

export namespace Elevate::Editor::Widgets
{
	void InputField(const char* label, std::string& data);
	void InputField(const char* label, const std::string& data);
}
