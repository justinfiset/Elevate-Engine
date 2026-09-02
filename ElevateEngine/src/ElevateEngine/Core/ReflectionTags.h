#pragma once

#include <string>
#include <variant>
#include <typeindex>
#include <type_traits>

#include <ElevateEngine/Core/AssetMetaData.h>

namespace Elevate
{
	struct EmptyTag {};

#ifdef EE_EDITOR_BUILD
	#define EE_EditorTag(x) x
#else
	#define EE_EditorTag(x) ::Elevate::EmptyTag{}
#endif

	// Assets tags

	struct AssetTag
	{
		AssetMetaData Meta;
		AssetTag(const AssetMetaData& meta);
	};
	struct CreateAssetMenuTag { const char* Path; const char* Ext; };
#define EE_CreateAssetMenu EE_EditorTag(CreateAssetMenuTag{})
	struct AssetColorTag { float r, g, b; };
#define EE_AssetColor(r, g, b) EE_EditorTag((AssetColorTag{r, g, b}))

	using AssetOption = std::variant<
		CreateAssetMenuTag, AssetColorTag	
	>;

	template<typename T, typename... Args>
	inline AssetMetaData BuildAssetMetaData(const std::string& name, const std::string& ext, Args&&... args)
	{
		AssetMetaData meta;
		meta.TypeName = name;
		meta.Extension = ext;
		meta.TypeIndex = typeid(T);

		auto processTag = [&meta](const auto& tag)
		{
			using Tag = std::decay_t<decltype(tag)>;
			if constexpr (std::is_same_v<Tag, CreateAssetMenuTag>)
			{
				meta.Flags |= AssetFlags::CreateAssetMenu;
			}
			else if constexpr (std::is_same_v<Tag, AssetColorTag>)
			{
				meta.AssetColor = glm::vec4(tag.r, tag.g, tag.b, 1.0f);
			}
		};
		(..., processTag(args));

		return meta;
	}

#define EE_Asset(name, extension, ...) EE_EditorTag(AssetTag{BuildAssetMetaData<ThisType>(name, extension, ##__VA_ARGS__)})

	// Editor Analyser Tags

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
		AssetTag, // Allow specification for assets
		// Analyser Panel
		HideInInspectorTag, EditorIconTag,
		FlattenTag, DisplayNameTag, TooltipTag, ReadOnlyTag, ColorTag,
		// Serialization
		NoSerializeTag
	>;
}