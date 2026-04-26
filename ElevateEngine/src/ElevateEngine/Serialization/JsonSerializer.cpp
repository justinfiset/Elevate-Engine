#include "JsonSerializer.h"

namespace Elevate
{
	bool JsonSerializer::Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const
	{
		for (auto& property : fields)
		{
			if (property.IsPrimitive())
			{
				// directly set the value
			}
			else if (property.IsContainer())
			{
				// recursivly get the json
			}
		}
		return true;
	}

	bool JsonSerializer::Deserialize(const ByteBuffer& data, PropertySet& outFields)
	{
		return false;
	}
}
