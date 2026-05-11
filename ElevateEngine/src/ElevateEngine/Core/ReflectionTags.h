#pragma once

#include <string>
#include <variant>

namespace Elevate
{
    struct EmptyTag {};

#ifdef EE_EDITOR_BUILD
	#define EditorTag(x) x
#else
	#define EditorTag(x) ::Elevate::EmptyTag{}
#endif

	struct HideInInspectorTag {};
	#define HideInInspector EditorTag(HideInInspectorTag {})

	struct FlattenTag {};
	#define Flatten EditorTag(FlattenTag {})

	struct DisplayNameTag { const char* value; };
	#define DisplayName EditorTag(DisplayNameTag {})

	struct TooltipTag { const char* text; };
	#define Tooltip EditorTag(TooltipTag {})

	struct ReadOnlyTag {};
	#define ReadOnly EditorTag(ReadOnlyTag {})

	struct ColorTag {};
	#define ColorPicker EditorTag(ColorTag {})

	struct EditorIconTag { std::string Path; };
	#define EditorIcon(path) EditorTag(EditorIconTag{path})

	using FieldOption = std::variant<
		HideInInspectorTag, EditorIconTag,
		FlattenTag, DisplayNameTag, TooltipTag, ReadOnlyTag, ColorTag
	>;
}