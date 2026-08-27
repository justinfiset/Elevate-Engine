#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/TypeRegistry.h>

namespace Elevate
{
    struct EditorTypeTrait : public ITypeTrait
    {
        bool visible = true;
        std::string editorIconPath = "";
        bool isAsset = false;

        EditorTypeTrait() = default;

        template<typename T>
        EditorTypeTrait(std::type_identity<T>, const std::vector<FieldOption> &options)
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