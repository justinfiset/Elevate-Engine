module;

#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

export module Elevate.Editor.Types.EditorTypeTrait;

import Elevate.Core.Types.ITypeTrait;			// ITypeTrait
import Elevate.Core.Reflection.ReflectionTags;	// FieldOption

export namespace Elevate
{
	struct EditorTypeTrait : public ITypeTrait
	{
		bool visible = true;
		std::string editorIconPath = "";
		bool isAsset = false;

		EditorTypeTrait() = default;

		template<typename T>
		EditorTypeTrait(std::type_identity<T>, const std::vector<FieldOption>& options)
		{
			for (auto& option : options)
			{
				if (std::holds_alternative<HideInInspectorTag>(option))
				{
					visible = false;
				}
				else if (std::holds_alternative<EditorIconTag>(option))
				{
					editorIconPath = std::get<EditorIconTag>(option).Path;
				}
				else if (auto* assetTag = std::get_if<AssetTag>(&option))
				{
					isAsset = true;
				}
			}
		}
	};
}