#include "TypeLayout.h"

#include <ElevateEngine/Core/TypeField.h>
#include <ElevateEngine/Serialization/PropertyField.h>

namespace Elevate
{
    PropertyFlag GetFieldFlags(const TypeField& field)
    {
        // todo correctly set the flags
    }

    PropertySet CreateContainer(const TypeField& parent, const std::string& parentPath, uint16_t currentDepth);
    PropertySet CreateArrayPropertySet(const TypeField& parent, const std::string& parentPath, uint16_t currentDepth);

    void SetPropertyRawValue(const TypeField& field, PropertyField& prop, uint16_t currentDepth)
    {
        if (field.type == EngineDataType::Array)
        {
            prop.Value = PropertyContainer{ CreateArrayPropertySet(field, prop.Path, currentDepth + 1) };
            return;
        }

        if (field.data == nullptr) return;

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
            prop.Value = *reinterpret_cast<const double*>(field.data);
            break;
        case EngineDataType::String:
            prop.Value = *reinterpret_cast<const std::string*>(field.data);
            break;
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

            if (!parent.elementChildren.empty())
            {
                TypeField virtualChildField = parent;
                virtualChildField.children = parent.elementChildren;
                virtualChildField.data = elementDataPtr;

                elemProp.Value = PropertyContainer{ CreateContainer(virtualChildField, elemProp.Path, currentDepth + 1) };
            }
            else
            {
                TypeField virtualPrimitiveField = parent;
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

            const void* fieldDataPtr = (parent.data != nullptr)
                ? reinterpret_cast<const char*>(parent.data) + field.offset
                : nullptr;

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

    PropertySet TypeLayout::CaptureState()
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

            if (field.type == EngineDataType::Array)
            {
                prop.Value = PropertyContainer{ CreateArrayPropertySet(field, prop.Path, 1) };
            }
            else if (!field.children.empty())
            {
                prop.Value = PropertyContainer{ CreateContainer(field, prop.Path, 1) };
            }
            else
            {
                SetPropertyRawValue(field, prop, 0);
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
                    size_t count = field.GetArraySize ? field.GetArraySize(field.data) : 0;

                    for (size_t i = 0; i < arraySet.size() && i < count; ++i)
                    {
                        const auto& elemProp = arraySet[i];
                        const void* elementDataPtr = field.GetElementAddress ? field.GetElementAddress(field.data, i) : nullptr;
                        if (!elementDataPtr) continue;

                        if (!field.elementChildren.empty())
                        {
                            TypeField virtualChildField = field;
                            virtualChildField.children = field.elementChildren;
                            virtualChildField.data = elementDataPtr;
                            ApplyPropertyValues(virtualChildField.children, arraySet, elemProp.Path);
                        }
                        else
                        {
                            if (field.elementType == EngineDataType::Bool && std::holds_alternative<bool>(elemProp.Value))
                            {
                                *const_cast<bool*>(reinterpret_cast<const bool*>(elementDataPtr)) = std::get<bool>(elemProp.Value);
                            }
                            else if (field.elementType == EngineDataType::Int)
                            {
                                if (std::holds_alternative<int64_t>(elemProp.Value))
                                    *const_cast<int32_t*>(reinterpret_cast<const int32_t*>(elementDataPtr)) = static_cast<int32_t>(std::get<int64_t>(elemProp.Value));
                            }
                            else if (field.elementType == EngineDataType::Float && std::holds_alternative<double>(elemProp.Value))
                            {
                                *const_cast<float*>(reinterpret_cast<const float*>(elementDataPtr)) = static_cast<float>(std::get<double>(elemProp.Value));
                            }
                            else if (field.elementType == EngineDataType::Double && std::holds_alternative<double>(elemProp.Value))
                            {
                                *const_cast<double*>(reinterpret_cast<const double*>(elementDataPtr)) = std::get<double>(elemProp.Value);
                            }
                            else if (field.elementType == EngineDataType::String && std::holds_alternative<std::string>(elemProp.Value))
                            {
                                *const_cast<std::string*>(reinterpret_cast<const std::string*>(elementDataPtr)) = std::get<std::string>(elemProp.Value);
                            }
                        }
                    }
                }
            }
            else if (!field.children.empty())
            {
                ApplyPropertyValues(field.children, props, currentPath);
            }
            else
            {
                auto it = std::find_if(props.begin(), props.end(), [&currentPath](const PropertyField& p) {
                    return p.Path == currentPath;
                    });

                if (it != props.end() && field.data != nullptr)
                {
                    switch (field.type)
                    {
                    case EngineDataType::Bool:
                        if (std::holds_alternative<bool>(it->Value))
                            *const_cast<bool*>(reinterpret_cast<const bool*>(field.data)) = std::get<bool>(it->Value);
                        break;
                    case EngineDataType::Int:
                        if (std::holds_alternative<int64_t>(it->Value))
                            *const_cast<int32_t*>(reinterpret_cast<const int32_t*>(field.data)) = static_cast<int32_t>(std::get<int64_t>(it->Value));
                        break;
                    case EngineDataType::Float:
                        if (std::holds_alternative<double>(it->Value))
                            *const_cast<float*>(reinterpret_cast<const float*>(field.data)) = static_cast<float>(std::get<double>(it->Value));
                        break;
                    case EngineDataType::Double:
                        if (std::holds_alternative<double>(it->Value))
                            *const_cast<double*>(reinterpret_cast<const double*>(field.data)) = std::get<double>(it->Value);
                        break;
                    case EngineDataType::String:
                        if (std::holds_alternative<std::string>(it->Value))
                            *const_cast<std::string*>(reinterpret_cast<const std::string*>(field.data)) = std::get<std::string>(it->Value);
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
        ApplyPropertyValues(m_fields, props, "");
    }
}