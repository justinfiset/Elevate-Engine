#pragma once

#include <string>
#include <variant>

namespace Elevate
{
    struct EmptyTag {};

#ifdef EE_EDITOR_BUILD
	#define EE_EditorTag(x) x
#else
	#define EE_EditorTag(x) ::Elevate::EmptyTag{}
#endif

	struct HideInInspectorTag {};
	#define EE_HideInInspector EditorTag(HideInInspectorTag {})

	struct FlattenTag {};
	#define EE_Flatten EditorTag(FlattenTag {})

	struct DisplayNameTag { const char* value; };
	#define EE_DisplayName EditorTag(DisplayNameTag {})

	struct TooltipTag { const char* text; };
	#define EE_Tooltip EditorTag(TooltipTag {})

	struct ReadOnlyTag {};
	#define EE_ReadOnly EditorTag(ReadOnlyTag {})

	struct ColorTag {};
	#define EE_ColorPicker EditorTag(ColorTag {})

	struct EditorIconTag { std::string Path; };
	#define EE_EditorIcon(path) EditorTag(EditorIconTag{path})

	struct NoSerializeTag {};
	#define EE_NoSerialize EditorTag(NoSerializeTag {})

	using FieldOption = std::variant<
		EmptyTag, // To allow empty types depending on compilation settings
		HideInInspectorTag, EditorIconTag,
		FlattenTag, DisplayNameTag, TooltipTag, ReadOnlyTag, ColorTag,
		NoSerializeTag
	>;
}