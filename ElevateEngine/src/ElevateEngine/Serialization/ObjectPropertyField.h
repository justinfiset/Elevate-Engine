#pragma once
#include <string>
#include <cstdint>
#include <variant>
#include <vector>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Byte.h>

namespace Elevate
{
	struct PropertyField;
    using PropertySet = std::vector<PropertyField>;

    struct PropertyContainer
    {
        PropertySet Children;
    };

    using PropertyValue = std::variant<
        bool,
        int64_t,            // Covers all int.
        double,             // Covers float and double
        std::string,
        ByteBuffer,         // Raw data for custom types
        PropertyContainer   // For recusrive structs
    >;

    enum PropertyFlag
    {
        None = 0,
        NoSave = 1 << 1
    };

#ifdef EE_EDITOR_BUILD
    struct PropertyEditorTrait
    {
        std::string DisplayName;
        std::string Tooltip;
        std::string IconPath;
        bool IsColor = false;
        bool IsReadOnly = false;
        bool IsHidden;
        bool IsFlatten = false;
    };
#endif

    class PropertyValue
    {
    public:

    private:
        PropertyRawVariant m_variant;
    };

	struct PropertyField
	{
	public:
        std::string Name;
		std::string Path;
		uint16_t Depth = 0;
        EngineDataType Type = EngineDataType::Unknown;
        uint16_t Flags = PropertyFlag::None;

        size_t Offset = 0;
        size_t Size = 0;
        PropertyValue Value;

#ifdef EE_EDITOR_BUILD
        std::shared_ptr<PropertyEditorTrait> EditorTrait;
#endif

        bool IsContainer() const
        {
            return std::holds_alternative<PropertyContainer>(Value);
        }

        bool IsPrimitive() const
        {
            return !IsContainer() && !std::holds_alternative<ByteBuffer>(Value);
        }

        const PropertySet& GetChildren() const
        {
            return std::get<PropertyContainer>(Value).Children;
        }
	};
}