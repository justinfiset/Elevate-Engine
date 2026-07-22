#include "TypeLayout.h"

#include <ElevateEngine/Core/TypeField.h>
#include <ElevateEngine/Serialization/PropertyField.h>

namespace Elevate
{
    PropertyFlag GetFieldFlags(const TypeField& field)
    {
        // Todo : complete this function to retrieve the flags using the traits
        return PropertyFlag::None;
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
            // todo : save as bytebuffer if the type is unknown
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
}