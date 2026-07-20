#pragma once

#include <ElevateEngine/Serialization/Serializer.h>

namespace Elevate
{
	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer() = default;
		virtual ~JsonSerializer() = default;

		virtual bool Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const override;
		virtual bool Deserialize(const ByteBuffer& data, PropertySet& outFields) override;
	};
}