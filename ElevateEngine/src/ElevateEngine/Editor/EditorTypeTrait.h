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
        
        // Assets
        bool isAsset = false;
        AssetMetaData assetMeta;
        std::function<std::shared_ptr<EEObject>()> factory = nullptr;

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
                    assetMeta = assetTag->Meta;

                    if constexpr (!std::is_base_of_v<EEObject, T>)
                    {
                        EE_ERROR("Editor : Type is not derived from EEObject : {}", typeid(T).name());
                    }
                    if constexpr (std::is_abstract_v<T>)
                    {
                        EE_ERROR("Editor : Type is abstract : {}", typeid(T).name());
                    }
                    if constexpr (!std::is_default_constructible_v<T>)
                    {
                        EE_ERROR("Editor : Type has no default constructor : {}", typeid(T).name());
                    }

                    factory = []() -> std::shared_ptr<EEObject> {
                        if constexpr (std::is_base_of_v<EEObject, T> &&
                            !std::is_abstract_v<T> &&
                            std::is_default_constructible_v<T>)
                        {
                            return std::make_shared<T>();
                        }
                        else
                        {
                            return nullptr;
                        }
                    };
                }
            }
        }
    };
}