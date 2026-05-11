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
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

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
        constexpr EngineDataType type = DeduceEngineDataType<FieldType>();
        FieldMeta meta;
        for (auto&& opt : options) {
            if (std::holds_alternative<FlattenTag>(opt)) {
                meta.flatten = true;
            }
            else if (std::holds_alternative<DisplayNameTag>(opt)) {
                meta.displayName = std::get<DisplayNameTag>(opt).value;
            }
            else if (std::holds_alternative<TooltipTag>(opt)) {
                meta.tooltip = std::get<TooltipTag>(opt).text;
            }
            else if (std::holds_alternative<ReadOnlyTag>(opt)) {
                meta.readOnly = true;
            }
            else if (std::holds_alternative<ColorTag>(opt)) {
                meta.isColor = true;
            }   
        }

        std::string cleanedName = GetCleanedName(name);

#ifdef EE_REGISTRY_LOG
        EE_TRACE(" --> Exposed field : %s flatten=%d  displayName=%s", cleanedName.c_str(), meta.flatten, meta.displayName.c_str());
#endif

        auto& customFields = GetCustomComponentFields();
        std::type_index ti = typeid(FieldType);
        size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<Class const volatile*>(0)->*member));

        ComponentField field;
        if (customFields.find(ti) != customFields.end())
        {
            field = ComponentField(cleanedName, EngineDataType::Custom, offset, meta.displayName, customFields[ti]);
            field.flatten = meta.flatten;
        }
        else
        {
            field = ComponentField(cleanedName, type, offset, meta.displayName);
        }
        field.isColor = meta.isColor;
        field.tooltip = meta.tooltip;
        field.readOnly = meta.readOnly;
        CompilationClassFieldStack().push_back(field);
    }
}