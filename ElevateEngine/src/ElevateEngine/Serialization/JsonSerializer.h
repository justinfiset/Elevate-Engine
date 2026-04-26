#pragma once

#include <ElevateEngine/Serialization/Serializer.h>

namespace Elevate
{
	class JsonSerializer : public Serializer
	{
		virtual bool Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const = 0;
		virtual bool Deserialize(const ByteBuffer& data, PropertySet& outFields) = 0;
	};
}