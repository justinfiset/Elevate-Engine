#include "JsonSerializer.h"
#include <variant>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace Elevate
{
	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...) -> overload<Ts...>;

	void SerializeRecursive(const PropertySet& fields, rapidjson::Value& parentObj, rapidjson::Document::AllocatorType& allocator)
	{
		for (const auto& property : fields)
		{
			rapidjson::Value key(property.Name.c_str(), allocator);

			rapidjson::Value val = std::visit(overload{
				[&](bool v) { return rapidjson::Value(v); },
				[&](int64_t v) { return rapidjson::Value(v); },
				[&](double v) { return rapidjson::Value(v); },
				[&](const std::string& v) {
					return rapidjson::Value(v.c_str(), allocator);
				},
				[&](const ByteBuffer& v) {
					return rapidjson::Value(
						reinterpret_cast<const char*>(v.data()),
						static_cast<rapidjson::SizeType>(v.size()),
						allocator
					);
				},
				[&](const PropertySet& v) {
					rapidjson::Value childObj(rapidjson::kObjectType);
					SerializeRecursive(v, childObj, allocator);
					return childObj;
				}
			}, property.Value);

			parentObj.AddMember(key, val, allocator);
		}
	}

	bool JsonSerializer::Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const
	{
		rapidjson::Document doc;
		doc.SetObject();

		SerializeRecursive(fields, doc, doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		const std::byte* dataPtr = reinterpret_cast<const std::byte*>(buffer.GetString());
		outBuffer.assign(dataPtr, dataPtr + buffer.GetSize());
		return true;
	}

	bool JsonSerializer::Deserialize(const ByteBuffer& data, PropertySet& outFields)
	{
		return false;
	}
}
