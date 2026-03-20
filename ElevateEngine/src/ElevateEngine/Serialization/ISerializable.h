#pragma once
#include <string>
#include <cstdint>
#include <vector>

namespace Elevate
{
	/**
	 * Simple interface used to serialize and deserialize using the Engine's common serialization system for custom types.
	 */
	class ISerializable
	{
	public:
		virtual ~ISerializable() = default;
		virtual std::vector<uint8_t> Serialize() const = 0;
		virtual void Deserialize(const std::vector<uint8_t>& data) = 0;
	};
}