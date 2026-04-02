#pragma once
#include <string>
#include <cstdint>
#include <variant>
#include <vector>

#include <ElevateEngine/Core/Data.h>

namespace Elevate
{
	struct PropertyField;
    using PropertySet = std::vector<PropertyField>;

    using PropertyValue = std::variant<
        bool,
        int64_t,              // Covers all int.
        double,               // Covers float and double
        std::string,
        ByteBuffer, // Raw data for custom types
        PropertySet // For recusrive structs
    >;

	enum PropertyFlag
	{
        None = 0,
        ReadOnly = 1 << 0,
        Hidden = 1 << 1,
        NoSave = 1 << 2
	};

	struct PropertyField
	{
	public:
        std::string Name;
		std::string Path;
		uint16_t Depth = 0;
        EngineDataType Type = EngineDataType::Unknown;
        PropertyValue Value;
        uint16_t Flags = PropertyFlag::None;

        bool IsContainer() const
        {
            return std::holds_alternative<PropertySet>(Value);
        }

        bool IsPrimitive() const
        {
            return !IsContainer() && !std::holds_alternative<ByteBuffer>(Value);
        }

        const PropertySet& GetChildren() const
        {
            return std::get<PropertySet>(Value);
        }
	};
}