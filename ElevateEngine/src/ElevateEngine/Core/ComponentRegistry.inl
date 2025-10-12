#pragma once

#include "ComponentRegistry.h"

#include <entt/entt.hpp>

#include <ElevateEngine/Scene/ScenePrivate.h>

namespace Elevate
{
    template<typename T>
    static void ComponentRegistry::Register(const std::string& name, EECategory category, std::vector<FieldOption>& options) {
        std::type_index ti(typeid(T));

        bool visible = true;
        for (FieldOption& option : options)
        {
            if (std::holds_alternative<HideInInspectorTag>(option))
            {
                visible = false;
            }
        }

        GetEntries().emplace(ti, Entry{
            name,
            ti,
            category,
            [](std::weak_ptr<GameObject> go) -> Component* {
                if (std::shared_ptr<GameObject> obj = go.lock()) {
                    return obj->GetComponent<T>();
                }
                return nullptr;
            },
            [](std::weak_ptr<GameObject> go) -> Component* {
                if (std::shared_ptr<GameObject> obj = go.lock()) {
                    return &obj->AddComponent<T>();
                }
                return nullptr;
            },
            [](std::weak_ptr<GameObject> go) -> void {
                if (std::shared_ptr<GameObject> obj = go.lock()) {
                    obj->RemoveComponent<T>();
                }
            },
            visible
        });
    }

    template<typename Class, typename FieldType>
    static void ComponentRegistry::AddProperty(FieldType Class::* member, const std::string& name, std::initializer_list<FieldOption> options)
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
        EE_TRACE(" --> Exposed field : %s flatten=%d  displayName=%s", cleanedName.c_str(), meta.flatten, meta.displayName.c_str());

        auto& customFields = GetCustomComponentFields();
        std::string typeName = typeid(FieldType).name();
        size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<Class const volatile*>(0)->*member));

        ComponentField field;
        if (customFields.find(typeName) != customFields.end())
        {
            field = ComponentField(cleanedName, EngineDataType::Custom, offset, meta.displayName, customFields[typeName]);
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