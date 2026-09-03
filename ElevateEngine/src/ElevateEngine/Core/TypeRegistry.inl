#pragma once
#include <ElevateEngine/Core/TypeRegistry.h>

#include <entt/entt.hpp>
#include <initializer_list>
#include <string>
#include <typeindex>
#include <variant>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/EEObject.h>

#include "ElevateEngine/Core/ReflectionTags.h"

#ifdef EE_EDITOR_BUILD
namespace Elevate
{
    struct EditorTypeTrait;
}
#endif

namespace Elevate
{
    template<typename T>
    void TypeRegistry::Register(const std::string& name, const std::vector<FieldOption>& options)
    {
        std::type_index ti(typeid(T));
        auto& entry = GetEntries()[ti];
        entry.name = name;
        entry.type = ti;

        if constexpr (std::is_base_of_v<EEObject, T> && !std::is_abstract_v<T> && std::is_default_constructible_v<T>)
        {
            entry.factory = []() -> std::shared_ptr<EEObject>
            {
                return std::make_shared<T>();
            };
        }

#ifdef EE_EDITOR_BUILD
        // Passing T using a type_identity simple placeholder.
        AddTrait<T, EditorTypeTrait>(std::type_identity<T>{}, options);
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

        alignas(Class) char dummyBuffer[sizeof(Class)];
        Class* dummyObj = reinterpret_cast<Class*>(dummyBuffer);
        size_t offset = static_cast<size_t>(
            reinterpret_cast<const char*>(&(dummyObj->*member)) - dummyBuffer
            );

        TypeField field;

        if constexpr (is_engine_array_v<CleanedFieldT>)
        {
            field = TypeField(name, EngineDataType::Array, offset, meta.displayName);

            using ElementType = typename CleanedFieldT::value_type;
            field.elementType = DeduceEngineDataType<ElementType>();

            if constexpr (is_ee_object_ptr_v<ElementType>)
            {
                using TargetT = ee_ptr_target_t<ElementType>;
                field.targetType = typeid(TargetT);
            }
            else if constexpr (std::is_class_v<ElementType> && !std::is_same_v<ElementType, std::string>)
            {
                auto& customFields = GetReflectedTypes();
                std::type_index ti = typeid(ElementType);
                auto it = customFields.find(ti);
                if (it != customFields.end()) {
                    field.elementChildren = it->second;
                }
            }

            field.GetArraySize = [](const void* vecPtr) -> size_t {
                if (!vecPtr) return 0;
                const auto* vec = static_cast<const CleanedFieldT*>(vecPtr);
                return vec->size();
                };

            field.GetElementAddress = [](const void* vecPtr, size_t index) -> const void* {
                if (!vecPtr) return nullptr;
                const auto* vec = static_cast<const CleanedFieldT*>(vecPtr);
                if (index >= vec->size()) return nullptr;
                return static_cast<const void*>(&(*vec)[index]);
                };

            field.ResizeArray = [](void* vecPtr, size_t newSize) {
                if (!vecPtr) return;
                auto* vec = static_cast<CleanedFieldT*>(vecPtr);
                vec->resize(newSize);
                };
        }
        else if constexpr (is_ee_object_ptr_v<CleanedFieldT>)
        {
            field = TypeField(name, EngineDataType::ObjectPtr, offset, meta.displayName);
            using TargetT = ee_ptr_target_t<CleanedFieldT>;
            field.targetType = typeid(TargetT);
        }
        else if (type == EngineDataType::Custom)
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
        else
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