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

	struct CreateAssetMenuTag { const char* Path; const char* Ext; };
#define EE_CreateAssetMenu(path, ext) EE_EditorTag((CreateAssetMenuTag{path, ext}))

	struct HideInInspectorTag {};
#define EE_HideInInspector EE_EditorTag(HideInInspectorTag {})

	struct FlattenTag {};
#define EE_Flatten EE_EditorTag(FlattenTag {})

	struct DisplayNameTag { const char* value; };
#define EE_DisplayName EE_EditorTag(DisplayNameTag {})

	struct TooltipTag { const char* text; };
#define EE_Tooltip EE_EditorTag(TooltipTag {})

	struct ReadOnlyTag {};
#define EE_ReadOnly EE_EditorTag(ReadOnlyTag {})

	struct ColorTag {};
#define EE_ColorPicker EE_EditorTag(ColorTag {})

	struct EditorIconTag { const char* Path; };
#define EE_EditorIcon(path) EE_EditorTag(EditorIconTag{path})

	struct NoSerializeTag {};
#define EE_NoSerialize EE_EditorTag(NoSerializeTag {})

	using FieldOption = std::variant<
		EmptyTag, // To allow empty types depending on compilation settings
		CreateAssetMenuTag,
		HideInInspectorTag, EditorIconTag,
		FlattenTag, DisplayNameTag, TooltipTag, ReadOnlyTag, ColorTag,
		NoSerializeTag
	>;
}