module;

#include <chrono>
#include <cstdint>
#include <string>
#include <utility>

#include <glm/vec4.hpp>

export module Elevate.Editor.Message;

import Elevate.Editor.Colors;

export namespace Elevate
{
	namespace Editor
	{
		struct EditorMessage
		{
			enum Type : uint8_t
			{
				Error,
				Warning,
				Message,
				Success,
				Information
			};

			std::string message;
			std::chrono::system_clock::time_point timestamp;
			Type type;

			EditorMessage(std::string message, Type type)
				: message(std::move(message)), timestamp(std::chrono::system_clock::now()), type(type) {
			}

			glm::vec4 GetColor() const
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
		};
	}
}
