#pragma once

#include <string>
#include <vector>
#include <functional>

#include <ElevateEngine/Core/Data.h>

namespace Elevate
{
    struct TypeField
    {
        std::string name;
        std::string displayName;
        std::string tooltip;
        EngineDataType type;
        const void* data = nullptr;
        size_t offset = 0;
        uint32_t size = 0;
        bool flatten = false;
        bool readOnly = false;
        bool isColor = false;
        std::vector<TypeField> children;

        EngineDataType elementType = EngineDataType::Custom;
        std::vector<TypeField> elementChildren;

        std::function<size_t(const void*)> GetArraySize;
        std::function<const void* (const void* instance, size_t index)> GetElementAddress;

        TypeField() = default;

        TypeField(
            const std::string& name,
            EngineDataType type,
            size_t offset,
            const std::string& displayName = ""
        )
            : name(name), type(type), offset(offset), displayName(displayName)
        {
            if (type == EngineDataType::Custom || type == EngineDataType::Array)
            {
                size = 0;
            }
            else
            {
                size = GetDataTypeSize(type);
            }
        }

        TypeField(
            const std::string& name,
            EngineDataType type,
            size_t offset,
            const std::string& displayName,
            const std::vector<TypeField>& childrenFields
        )
            : name(name), type(type), offset(offset), displayName(displayName), children(childrenFields)
        {
            if (type == EngineDataType::Custom || type == EngineDataType::Array)
            {
                size = 0;
            }
            else
            {
                size = GetDataTypeSize(type);
            }
        }

        TypeField(const std::string& name, EngineDataType type, const void* dataPtr)
            : name(name), type(type), data(dataPtr) {
        }

        TypeField(const TypeField& original, const void* dataPtr)
            : name(original.name), type(original.type), offset(original.offset), size(original.size),
            displayName(original.displayName), flatten(original.flatten), readOnly(original.readOnly),
            isColor(original.isColor), data(dataPtr),
            elementType(original.elementType), elementChildren(original.elementChildren),
            GetArraySize(original.GetArraySize), GetElementAddress(original.GetElementAddress)
        {
            for (const auto& child : original.children)
            {
                const void* childDataPtr = reinterpret_cast<const char*>(dataPtr) + child.offset;
                children.emplace_back(child, childDataPtr);
            }
        }

        inline const std::string& GetDisplayName() const
        {
            if (displayName.empty())
                return name;
            return displayName;
        }

        inline bool HasChildrens() { return !children.empty() || type == EngineDataType::Array; }
    };
}
