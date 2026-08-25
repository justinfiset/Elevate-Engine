#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/TypeRegistry.h>

namespace Elevate
{
    struct EditorTypeTrait : public ITypeTrait
    {
        bool visible = true;
        std::string editorIconPath = "";

        bool isCreatableAsset = false;
        std::string menuPath = "";
        std::string extension = "";
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
                else if (std::holds_alternative<CreateAssetMenuTag>(option))
                {
                    const auto& tag = std::get<CreateAssetMenuTag>(option);
                    menuPath = tag.Path;
                    extension = tag.Ext;
                    isCreatableAsset = true;

                    factory = []() -> std::shared_ptr<EEObject> {
                        // Only create a factory if the object is derived from EEObject and complete
                        if constexpr (std::is_base_of_v<EEObject, T> &&
                                    !std::is_abstract_v<T> &&
                                    std::is_default_constructible_v<T>)
                        {
                            return std::make_shared<T>();
                        }
                        return nullptr;
                    };
                }
            }
        }
    };
}