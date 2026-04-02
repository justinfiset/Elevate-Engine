#pragma once
#include <string>
#include <cstdint>
#include <vector>

#include <ElevateEngine/Core/Byte.h>

namespace Elevate
{
	/**
	 * Simple interface used to serialize and deserialize using the Engine's common serialization system for custom types.
	 */
	class ISerializable
	{
	public:
		virtual ~ISerializable() = default;
		virtual ByteBuffer Serialize() const = 0;
		virtual void Deserialize(const ByteBuffer& data) = 0;
	};
}