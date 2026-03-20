#pragma once
#include <string>
#include <cstdint>
#include <variant>
#include <vector>

#include <ElevateEngine/Core/Data.h>

namespace Elevate
{
	struct PropertyField;

    using PropertyValue = std::variant<
        bool,
        int64_t,              // Covers all int.
        double,               // Covers float and double
        std::string,
        std::vector<uint8_t>, // Raw data for custom types
        std::vector<PropertyField> // For recusrive structs
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
	};
}