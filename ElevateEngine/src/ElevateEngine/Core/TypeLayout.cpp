#include "TypeLayout.h"

#include <ElevateEngine/Serialization/ObjectPropertyField.h>

namespace Elevate
{
    PropertyFlag GetFieldFlags(const TypeField& field)
    {
        // Todo : complete this function to retrieve the flags using the traits
        return PropertyFlag::None;
    }

    void SetPropertyRawValue(const TypeField& field, PropertyField& prop)
    {
        if (field.data != nullptr)
        {
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

            if (!field.children.empty())
            {
                prop.Value = PropertyContainer{ CreateContainer(field, prop.Path, currentDepth + 1) };
            }
            else
            {
                SetPropertyRawValue(field, prop);
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
            
            // If the current field has childrens (eg. a struct)
            if (!field.children.empty())
            {
                prop.Value = PropertyContainer{ CreateContainer(field, prop.Path, 1) };
            }
            else
            {
                SetPropertyRawValue(field, prop);
            }

            set.push_back(prop);
        }

        return set;
    }
}
