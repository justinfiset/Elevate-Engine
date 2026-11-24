module;


#include <chrono>
#include <string>
#include <utility>

#include <glm/vec4.hpp>

module Elevate.Editor.Message;

import Elevate.Editor.Colors;

namespace Elevate::Editor
{
	EditorMessage::EditorMessage(std::string message, Type type)
		: message(std::move(message)), type(type), timestamp(std::chrono::system_clock::now())
	{
	}

	glm::vec4 EditorMessage::GetColor() const
	{
		switch (type)
		{
		case Error:       return Colors::Error();
		case Warning:     return Colors::Warning();
		case Message:     return Colors::Message();
		case Success:     return Colors::Success();
		case Information: return Colors::Information();
		}
		return Colors::Information();
	}
}
