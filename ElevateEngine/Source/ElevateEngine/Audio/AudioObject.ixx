module;

#include <cstdint>
#include <string>

#include <glm/vec3.hpp>

export module Elevate.Audio.Object;

export namespace Elevate
{
	class IAudioObject
	{
	public:
		virtual ~IAudioObject() = default;

		virtual uint32_t GetObjectId() = 0;
		virtual std::string GetName() const = 0;
		virtual glm::vec3& GetPosition() = 0;
		virtual glm::vec3 GetUp() const = 0;
		virtual glm::vec3 GetForward() const = 0;
	};
}
