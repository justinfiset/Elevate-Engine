export module Elevate.Serialization.JsonSerializer;

import Elevate.Serialization.ISerializer;
import Elevate.Serialization.PropertyField;
import Elevate.Foundations.Bytes;

export namespace Elevate
{
	class JsonSerializer : public ISerializer
	{
	public:
		JsonSerializer() = default;
		virtual ~JsonSerializer() = default;

		virtual bool Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const override;
		virtual bool Deserialize(const ByteBuffer& data, PropertySet& outFields) override;
	};
}