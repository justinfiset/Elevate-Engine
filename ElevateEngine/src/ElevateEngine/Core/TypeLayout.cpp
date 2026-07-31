#include "TypeLayout.h"

#include <ElevateEngine/Core/TypeField.h>
#include <ElevateEngine/Serialization/PropertyField.h>

namespace Elevate
{
    PropertyFlag GetFieldFlags(const TypeField& field)
    {
        return PropertyFlag::None;
    }

    PropertySet CreateContainer(const TypeField& parent, const std::string& parentPath, uint16_t currentDepth);
    PropertySet CreateArrayPropertySet(const TypeField& parent, const std::string& parentPath, uint16_t currentDepth);

    bool IsValidPointer(const void* ptr)
    {
        if (ptr == nullptr) return false;
        uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
        if (p < 0x10000 || p == 0xFFFFFFFFFFFFFFE7 || p > 0x7FFFFFFFFFFF) return false;
        return true;
    }

    void SetPropertyRawValue(const TypeField& field, PropertyField& prop, uint16_t currentDepth)
    {
        if (field.type == EngineDataType::Array)
        {
            prop.Value = PropertyContainer{ CreateArrayPropertySet(field, prop.Path, currentDepth + 1) };
            return;
        }

        if (!IsValidPointer(field.data)) return;

        switch (field.type)
        {
        case EngineDataType::Bool:
            prop.Value = *reinterpret_cast<const bool*>(field.data);
            break;
        case EngineDataType::Int:
            prop.Value = static_cast<int64_t>(*reinterpret_cast<const int32_t*>(field.data));
            break;
        case EngineDataType::Float:
            prop.Value = static_cast<double>(*reinterpret_cast<const float*>(field.data));
            break;
        case EngineDataType::Double:
            prop.Value = *reinterpret_cast<double*>(const_cast<void*>(field.data));
            break;
        case EngineDataType::String:
        {
            if (IsValidPointer(field.data))
            {
                const auto* strPtr = reinterpret_cast<const std::string*>(field.data);
                const char* cstr = strPtr->c_str();
                if (IsValidPointer(cstr))
                {
                    prop.Value = *strPtr;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    PropertySet CreateArrayPropertySet(const TypeField& parent, const std::string& parentPath, uint16_t currentDepth)
    {
        PropertySet arraySet;

        size_t count = parent.GetArraySize ? parent.GetArraySize(parent.data) : 0;
        if (count == 0 || !parent.GetElementAddress) return arraySet;

        for (size_t i = 0; i < count; ++i)
        {
            PropertyField elemProp;
            std::string indexStr = "[" + std::to_string(i) + "]";

            elemProp.Name = indexStr;
            elemProp.Path = parentPath + indexStr;
            elemProp.Depth = currentDepth;
            elemProp.Type = parent.elementType;

            const void* elementDataPtr = parent.GetElementAddress(parent.data, i);
            if (!IsValidPointer(elementDataPtr)) continue;

            if (!parent.elementChildren.empty())
            {
                TypeField virtualChildField;
                virtualChildField.name = indexStr;
                virtualChildField.type = EngineDataType::Custom;
                virtualChildField.data = elementDataPtr;
                virtualChildField.children = parent.elementChildren;

                elemProp.Value = PropertyContainer{ CreateContainer(virtualChildField, elemProp.Path, currentDepth + 1) };
            }
            else
            {
                TypeField virtualPrimitiveField;
                virtualPrimitiveField.name = indexStr;
                virtualPrimitiveField.type = parent.elementType;
                virtualPrimitiveField.data = elementDataPtr;

                SetPropertyRawValue(virtualPrimitiveField, elemProp, currentDepth);
            }

            arraySet.push_back(elemProp);
        }

        return arraySet;
    }

    PropertySet CreateContainer(const TypeField& parent, const std::string& parentPath, uint16_t currentDepth)
    {
        PropertySet set;

        for (const TypeField& field : parent.children)
        {
            PropertyField prop;
            prop.Name = field.name;
            prop.Type = field.type;
            prop.Path = parentPath.empty() ? field.name : parentPath + "/" + field.name;
            prop.Depth = currentDepth;
            prop.Flags = GetFieldFlags(field);

            const void* fieldDataPtr = field.data;
            if (fieldDataPtr == nullptr && IsValidPointer(parent.data))
            {
                fieldDataPtr = reinterpret_cast<const char*>(parent.data) + field.offset;
            }

            if (!IsValidPointer(fieldDataPtr) && field.type != EngineDataType::Array && field.children.empty())
            {
                continue;
            }

            TypeField instantiatedField = field;
            instantiatedField.data = fieldDataPtr;

            if (field.type == EngineDataType::Array)
            {
                prop.Value = PropertyContainer{ CreateArrayPropertySet(instantiatedField, prop.Path, currentDepth + 1) };
            }
            else if (!field.children.empty())
            {
                prop.Value = PropertyContainer{ CreateContainer(instantiatedField, prop.Path, currentDepth + 1) };
            }
            else
            {
                SetPropertyRawValue(instantiatedField, prop, currentDepth);
            }

            set.push_back(prop);
        }

        return set;
    }

    PropertySet TypeLayout::CaptureState() const
    {
        PropertySet set;

        for (const TypeField& field : m_fields)
        {
            PropertyField prop;
            prop.Name = field.name;
            prop.Path = field.name;
            prop.Type = field.type;
            prop.Depth = 0;
            prop.Flags = GetFieldFlags(field);

            const void* resolvedData = field.data != nullptr ? field.data : (m_objectInstance != nullptr ? (reinterpret_cast<const char*>(m_objectInstance) + field.offset) : nullptr);

            if (!IsValidPointer(resolvedData) && field.type != EngineDataType::Array && field.children.empty())
            {
                continue;
            }

            TypeField instantiatedField = field;
            instantiatedField.data = resolvedData;

            if (field.type == EngineDataType::Array)
            {
                prop.Value = PropertyContainer{ CreateArrayPropertySet(instantiatedField, prop.Path, 1) };
            }
            else if (!field.children.empty())
            {
                prop.Value = PropertyContainer{ CreateContainer(instantiatedField, prop.Path, 1) };
            }
            else
            {
                SetPropertyRawValue(instantiatedField, prop, 0);
            }

            set.push_back(prop);
        }

        return set;
    }

    void ApplyPropertyValues(const std::vector<TypeField>& fields, const PropertySet& props, const std::string& parentPath)
    {
        for (const auto& field : fields)
        {
            std::string currentPath = parentPath.empty() ? field.name : parentPath + "/" + field.name;

            if (field.type == EngineDataType::Array)
            {
                auto it = std::find_if(props.begin(), props.end(), [&currentPath](const PropertyField& p) {
                    return p.Path == currentPath;
                    });

                if (it != props.end() && std::holds_alternative<PropertyContainer>(it->Value))
                {
                    const auto& arraySet = std::get<PropertyContainer>(it->Value).Children;

                    if (field.ResizeArray)
                    {
                        field.ResizeArray(const_cast<void*>(field.data), arraySet.size());
                    }

                    size_t count = field.GetArraySize ? field.GetArraySize(field.data) : 0;

                    for (size_t i = 0; i < arraySet.size() && i < count; ++i)
                    {
                        const auto& elemProp = arraySet[i];
                        const void* elementDataPtr = field.GetElementAddress ? field.GetElementAddress(field.data, i) : nullptr;
                        if (!IsValidPointer(elementDataPtr)) continue;

                        if (!field.elementChildren.empty())
                        {
                            std::vector<TypeField> instantiatedChildren;
                            for (const auto& childField : field.elementChildren)
                            {
                                const void* childDataPtr = IsValidPointer(elementDataPtr) ? reinterpret_cast<const char*>(elementDataPtr) + childField.offset : nullptr;
                                TypeField instChild = childField;
                                instChild.data = childDataPtr;
                                instantiatedChildren.push_back(instChild);
                            }

                            ApplyPropertyValues(instantiatedChildren, std::get<PropertyContainer>(elemProp.Value).Children, elemProp.Path);
                        }
                        else
                        {
                            if (!IsValidPointer(elementDataPtr)) continue;

                            if (field.elementType == EngineDataType::Bool && std::holds_alternative<bool>(elemProp.Value))
                            {
                                *reinterpret_cast<bool*>(const_cast<void*>(elementDataPtr)) = std::get<bool>(elemProp.Value);
                            }
                            else if (field.elementType == EngineDataType::Int)
                            {
                                if (std::holds_alternative<int64_t>(elemProp.Value))
                                    *reinterpret_cast<int32_t*>(const_cast<void*>(elementDataPtr)) = static_cast<int32_t>(std::get<int64_t>(elemProp.Value));
                            }
                            else if (field.elementType == EngineDataType::Float && std::holds_alternative<double>(elemProp.Value))
                            {
                                *reinterpret_cast<float*>(const_cast<void*>(elementDataPtr)) = static_cast<float>(std::get<double>(elemProp.Value));
                            }
                            else if (field.elementType == EngineDataType::Double && std::holds_alternative<double>(elemProp.Value))
                            {
                                *reinterpret_cast<double*>(const_cast<void*>(elementDataPtr)) = std::get<double>(elemProp.Value);
                            }
                            else if (field.elementType == EngineDataType::String && std::holds_alternative<std::string>(elemProp.Value))
                            {
                                *reinterpret_cast<std::string*>(const_cast<void*>(elementDataPtr)) = std::get<std::string>(elemProp.Value);
                            }
                        }
                    }
                }
            }
            else if (!field.children.empty())
            {
                auto it = std::find_if(props.begin(), props.end(), [&currentPath](const PropertyField& p) {
                    return p.Path == currentPath;
                });

                if (it != props.end() && std::holds_alternative<PropertyContainer>(it->Value))
                {
                    const auto& container = std::get<PropertyContainer>(it->Value);
                    const void* subStructDataPtr = field.data;

                    if (!IsValidPointer(field.data))
                    {
                        return;
                    }

                    if (field.ResizeArray)
                    {
                        field.ResizeArray(const_cast<void*>(field.data), container.Children.size());
                    }

                    std::vector<TypeField> instantiatedChildren;
                    for (const auto& childField : field.children)
                    {
                        const void* childDataPtr = IsValidPointer(subStructDataPtr) ? reinterpret_cast<const char*>(subStructDataPtr) + childField.offset : nullptr;
                        TypeField instChild = childField;
                        instChild.data = childDataPtr;
                        instantiatedChildren.push_back(instChild);
                    }

                    ApplyPropertyValues(instantiatedChildren, container.Children, currentPath);
                }
            }
            else
            {
                auto it = std::find_if(props.begin(), props.end(), [&currentPath](const PropertyField& p) {
                    return p.Path == currentPath;
                    });

                if (it != props.end() && IsValidPointer(field.data))
                {
                    switch (field.type)
                    {
                    case EngineDataType::Bool:
                        if (std::holds_alternative<bool>(it->Value))
                            *reinterpret_cast<bool*>(const_cast<void*>(field.data)) = std::get<bool>(it->Value);
                        break;
                    case EngineDataType::Int:
                        if (std::holds_alternative<int64_t>(it->Value))
                            *reinterpret_cast<int32_t*>(const_cast<void*>(field.data)) = static_cast<int32_t>(std::get<int64_t>(it->Value));
                        break;
                    case EngineDataType::Float:
                        if (std::holds_alternative<double>(it->Value))
                            *reinterpret_cast<float*>(const_cast<void*>(field.data)) = static_cast<float>(std::get<double>(it->Value));
                        break;
                    case EngineDataType::Double:
                        if (std::holds_alternative<double>(it->Value))
                            *reinterpret_cast<double*>(const_cast<void*>(field.data)) = std::get<double>(it->Value);
                        break;
                    case EngineDataType::String:
                        if (std::holds_alternative<std::string>(it->Value))
                            *reinterpret_cast<std::string*>(const_cast<void*>(field.data)) = std::get<std::string>(it->Value);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    void TypeLayout::ApplyState(const PropertySet& props)
    {
        std::vector<TypeField> instantiatedFields;
        for (const auto& field : m_fields)
        {
            const void* resolvedData = field.data;
            if (resolvedData == nullptr && IsValidPointer(m_objectInstance))
            {
                resolvedData = reinterpret_cast<const char*>(m_objectInstance) + field.offset;
            }
            TypeField instField = field;
            instField.data = resolvedData;
            instantiatedFields.push_back(instField);
        }
        ApplyPropertyValues(instantiatedFields, props, "");
    }
}