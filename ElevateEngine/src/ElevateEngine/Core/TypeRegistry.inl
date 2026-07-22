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
    void TypeRegistry::AddPropertyDirect(
        FieldType Class::* member,
        const std::string& name,
        std::initializer_list<FieldOption> options,
        std::vector<TypeField>& targetStack
    ) {
        using CleanedFieldT = std::decay_t<FieldType>;
        constexpr EngineDataType type = DeduceEngineDataType<CleanedFieldT>();

        FieldMeta meta;
        std::string cleanedName = GetCleanedName(name);
        meta.displayName = cleanedName;
        for (auto&& opt : options) {
            if (std::holds_alternative<FlattenTag>(opt)) { meta.flatten = true; }
            else if (std::holds_alternative<DisplayNameTag>(opt)) { meta.displayName = std::get<DisplayNameTag>(opt).value; }
            else if (std::holds_alternative<TooltipTag>(opt)) { meta.tooltip = std::get<TooltipTag>(opt).text; }
            else if (std::holds_alternative<ReadOnlyTag>(opt)) { meta.readOnly = true; }
            else if (std::holds_alternative<ColorTag>(opt)) { meta.isColor = true; }
        }

        size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<Class const volatile*>(0)->*member));
        TypeField field;

        if constexpr (is_engine_array_v<CleanedFieldT>) // manage std::vectors
        {
            field = TypeField(name, EngineDataType::Array, offset, meta.displayName);

            using ElementType = typename CleanedFieldT::value_type;
            field.elementType = DeduceEngineDataType<ElementType>();

            if constexpr (std::is_class_v<ElementType> && !std::is_same_v<ElementType, std::string>)
            {
                auto& customFields = GetReflectedTypes();
                std::type_index ti = typeid(ElementType);
                auto it = customFields.find(ti);
                if (it != customFields.end()) {
                    field.elementChildren = it->second;
                }
            }

            field.GetArraySize = [member](const void* instance) -> size_t {
                if (!instance) return 0;
                const auto* obj = static_cast<const Class*>(instance);
                return (obj->*member).size();
                };

            field.GetElementAddress = [member](const void* instance, size_t index) -> const void* {
                if (!instance) return nullptr;
                const auto* obj = static_cast<const Class*>(instance);
                const auto& vec = obj->*member;
                if (index >= vec.size()) return nullptr;
                return static_cast<const void*>(&(vec[index]));
                };
        }
        else if (type == EngineDataType::Custom) // Manage custom struct or classes
        {
            auto& customFields = GetReflectedTypes();
            std::type_index ti = typeid(CleanedFieldT);

            std::vector<TypeField> subFields;
            auto it = customFields.find(ti);
            if (it != customFields.end()) {
                subFields = it->second;
            }
            field = TypeField(name, EngineDataType::Custom, offset, meta.displayName, subFields);
        }
        else // Manage primitive types
        {   
            field = TypeField(name, type, offset, meta.displayName);
        }

        field.flatten = meta.flatten;
        field.isColor = meta.isColor;
        field.tooltip = meta.tooltip;
        field.readOnly = meta.readOnly;

        targetStack.push_back(field);
    }
}