#pragma once

#include "EditorColor.h"
#include <glm/vec4.hpp>

namespace Elevate::Editor
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

		/** The message content */
		std::string message;
		/** The time at wich this EditorMessage was created */
		std::chrono::system_clock::time_point timestamp;
		/** The message type for this message */
		Type type;

		EditorMessage(std::string message, Type type)
			: message(message), type(type), timestamp(std::chrono::system_clock::now()) { }

		inline glm::vec4 GetColor() const
		{
			// todo remove the hardcoded colors from here
			switch (type)
			{
			case Error:
				return Colors::Error;
			case Warning:
				return Colors::Warning;
			case Message:
				return Colors::Message;
			case Success:
				return Colors::Success;
			case Information:
				return Colors::Information;
			}
			return Colors::Information;
		}
	};
}