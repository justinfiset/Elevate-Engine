#pragma once
#include <ElevateEngine/Core/TypeRegistry.h>

namespace Elevate
{
    struct EditorTypeTrait : public ITypeTrait
    {
        bool visible = true;
        std::string editorIconPath = "";

        EditorTypeTrait() = default;

        EditorTypeTrait(const std::vector<FieldOption> &options)
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
            }
        }
    };
}