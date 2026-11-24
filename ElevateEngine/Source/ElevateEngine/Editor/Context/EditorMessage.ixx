module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <chrono>
#include <cstdint>
#include <string>

#include <glm/vec4.hpp>

export module Elevate.Editor.Message;

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

			/** The message content */
			std::string message;
			/** The time at wich this EditorMessage was created */
			std::chrono::system_clock::time_point timestamp;
			/** The message type for this message */
			Type type;

			EditorMessage(std::string message, Type type);
			glm::vec4 GetColor() const;
		};
	}
}
