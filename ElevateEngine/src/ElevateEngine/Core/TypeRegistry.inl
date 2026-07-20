#pragma once
#include <ElevateEngine/Core/TypeRegistry.h>

#include <entt/entt.hpp>
#include <initializer_list>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <variant>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/GameObject.inl>

// todo make editor only
#include <ElevateEngine/Core/TypeLayout.h>

#ifdef EE_EDITOR_BUILD
    #include <ElevateEngine/Editor/EditorTypeTrait.h>
#endif

namespace Elevate
{
    class Component;
    class GameObject;

    struct ColorTag;
    struct DisplayNameTag;
    struct EditorIconTag;
    struct FlattenTag;
    struct HideInInspectorTag;
    struct ReadOnlyTag;
    struct TooltipTag;
}


namespace Elevate
{
    template<typename T>
    void TypeRegistry::Register(const std::string& name, const std::vector<FieldOption>& options)
    {
        std::type_index ti(typeid(T));
        GetEntries().emplace(ti, Entry(name, ti));

#ifdef EE_EDITOR_BUILD
        AddTrait<T, EditorTypeTrait>(options);
#endif
    }

    template<typename Class, typename FieldType>
    void TypeRegistry::AddProperty(FieldType Class::* member, const std::string& name, std::initializer_list<FieldOption> options)
    {
        std::string cleanedName = GetCleanedName(name);

#ifdef EE_REGISTRY_LOG
        EE_TRACE(" --> Exposed field : %s flatten=%d  displayName=%s", cleanedName.c_str(), meta.flatten, meta.displayName.c_str());
#endif

        constexpr EngineDataType deducedType = 

        PropertyField field;
        field.Name = cleanedName;
        field.Path = cleanedName; // todo fix this to have a relevant path
        field.Depth = 0; // todo fix this to have the depth
        field.Type = DeduceEngineDataType<FieldType>();
        field.Flags = PropertyFlag::None;
        field.Value = 

        // todo : remove this if statment
        size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<Class const volatile*>(0)->*member));

        TypeField field;
        if (customFields.find(ti) != customFields.end())
        {
            field = TypeField(cleanedName, EngineDataType::Custom, offset, meta.displayName, customFields[ti]);
            field.flatten = meta.flatten;
        }
        else
        {
            field = TypeField(cleanedName, type, offset, meta.displayName);
        }

#ifdef EE_EDITOR_BUILD
        for (auto&& opt : options) {
            if (std::holds_alternative<FlattenTag>(opt)) {
                field.EditorTrait->IsFlatten = true;
            }
            else if (std::holds_alternative<DisplayNameTag>(opt)) {
                field.EditorTrait->DisplayName = std::get<DisplayNameTag>(opt).value;
            }
            else if (std::holds_alternative<TooltipTag>(opt)) {
                field.EditorTrait->Tooltip = std::get<TooltipTag>(opt).text;
            }
            else if (std::holds_alternative<ReadOnlyTag>(opt)) {
                field.EditorTrait->IsReadOnly = true;
            }
            else if (std::holds_alternative<ColorTag>(opt)) {
                field.EditorTrait->IsColor = true;
            }
            else if (std::holds_alternative<NoSerializeTag>(opt)) {
                field.Flags |= PropertyFlag::NoSave;
            }
        }
#endif

        CompilationClassFieldStack().push_back(field);
    }
}