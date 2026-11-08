module;

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

module Elevate.Core.Types.ValueCodec;

import Elevate.Foundations.Data;
import Elevate.Foundations.Enums;
import Elevate.Foundations.Guid;
import Elevate.Foundations.CoreLogger;
import Elevate.Core.Objects.ObjectPtr;

namespace Elevate
{
    template <class ValueType, class StoredType>
    ValueCodec MakeScalarCodec()
    {
        return {
            // Memory -> PropertyField
            [](const void *data, PropertyField &out)
            {
                out.Value = static_cast<StoredType>(*static_cast<const ValueType *>(data));
            },
            // PropertyField -> Memory
            [](void *data, const PropertyField &in)
            {
                const auto *value = std::get_if<StoredType>(&in.Value);

                if (!value)
                {
                    CoreLogger::Error("Failed to apply value codec: PropertyField does not contain the expected type.");
                    return false;
                }

                *static_cast<ValueType *>(data) = static_cast<ValueType>(*value);
                return true;
            }};
    }

    std::string ComponentName(size_t index, size_t count)
    {
        static constexpr const char *xyzw[4] = {"x", "y", "z", "w"};
        return (count <= 4) ? std::string(xyzw[index]) : std::to_string(index);
    }

    template <class VectorType, class ElementType, class StoredType, size_t Count, EngineDataType DataType>
    ValueCodec MakeGlmVecCodec()
    {
        return {
            // Memory -> PropertyField
            [](const void *data, PropertyField &out)
            {
                const ElementType *source = glm::value_ptr(*static_cast<const VectorType *>(data));

                PropertySet comps;
                comps.reserve(Count);

                for (size_t i = 0; i < Count; ++i)
                {
                    PropertyField compField;
                    compField.Name = ComponentName(i, Count);
                    compField.Path = out.Path + "/" + compField.Name;
                    compField.Type = DataType;
                    compField.Depth = out.Depth + 1;
                    compField.Value = static_cast<StoredType>(source[i]);

                    comps.push_back(std::move(compField));
                }

                out.Value = PropertyContainer{std::move(comps)};
            },
            // Memory -> PropertyField
            [](void *data, const PropertyField &in)
            {
                const auto *container = std::get_if<PropertyContainer>(&in.Value);
                if (!container)
                {
                    CoreLogger::Error("Failed to apply value codec: PropertyField does not contain a PropertyContainer.");
                    return false;
                }

                ElementType *destination = glm::value_ptr(*static_cast<VectorType *>(data));

                if (container->Children.size() < Count)
                {
                    CoreLogger::Error("ValueCodec: '{}' has {} components, expected {}.", in.Path, container->Children.size(), Count);
                    return false;
                }

                for (size_t i = 0; i < Count; ++i)
                {
                    if (const auto *value = std::get_if<StoredType>(&container->Children.at(i).Value))
                    {
                        destination[i] = static_cast<ElementType>(*value);
                    }
                    else
                    {
                        CoreLogger::Error("Failed to apply value codec: PropertyField component does not contain the expected type.");
                        return false;
                    }
                }

                return true;
            }};
    }

    template <int N>
    ValueCodec MakeFloatVecCodec()
    {
        return MakeGlmVecCodec<
            glm::vec<N, float>,
            float,
            double,
            N,
            static_cast<EngineDataType>(static_cast<int>(EngineDataType::Float) + N - 1)>();
    }

    template <int N>
    ValueCodec MakeIntVecCodec()
    {
        return MakeGlmVecCodec<
            glm::vec<N, int>,
            int,
            int64_t,
            N,
            static_cast<EngineDataType>(static_cast<int>(EngineDataType::Int) + N - 1)>();
    }

    // --------------------------------------------------------------------
    // Guid stocked as string
    // --------------------------------------------------------------------
    ValueCodec MakeGuidCoded()
    {
        return {
            // Memory -> PropertyField
            [](const void *data, PropertyField &out)
            {
                out.Value = static_cast<const Guid *>(data)->ToString();
            },
            // PropertyField -> Memory
            [](void *data, const PropertyField &in)
            {
                const auto *value = std::get_if<std::string>(&in.Value);

                if (!value)
                {
                    CoreLogger::Error("Failed to apply value codec: PropertyField does not contain the expected type.");
                    return false;
                }

                *static_cast<Guid *>(data) = Guid::FromString(*value);
                return true;
            }};
    }

    // --------------------------------------------------------------------
    // Enums stocked as int
    // --------------------------------------------------------------------
    ValueCodec MakeEnumCodec()
    {
        return {
            // Memory -> PropertyField
            [](const void *data, PropertyField &out)
            {
                out.Value = *static_cast<const EnumType *>(data);
            },
            // PropertyField -> Memory
            [](void *data, const PropertyField &in)
            {
                const auto *value = std::get_if<EnumType>(&in.Value);

                if (!value)
                {
                    CoreLogger::Error("Failed to apply value codec: PropertyField does not contain the expected type.");
                    return false;
                }

                *static_cast<EnumType *>(data) = *value;
                return true;
            }};
    }

    // --------------------------------------------------------------------
    // ObjectPtr stocked as guid string
    // --------------------------------------------------------------------
    ValueCodec MakeObjectPtrCodec()
    {
        return {
            [](const void *data, PropertyField &out)
            {
                out.Value = static_cast<const IEEObjectPtr *>(data)->GetGuid().ToString();
            },
            [](void *data, const PropertyField &in)
            {
                const auto *value = std::get_if<std::string>(&in.Value);
                if (!value)
                {
                    CoreLogger::Error("Failed to apply value codec: PropertyField does not contain the expected type.");
                    return false;
                }

                // TODO: Implement ObjectPtr restoration from guid string
                CoreLogger::Error("ObjectPtr restoration from guid string is not implemented.");
                return false;
            }};
    }

    constexpr size_t EngineDataTypeCount = static_cast<size_t>(EngineDataType::Unknown) + 1;

    std::array<ValueCodec, EngineDataTypeCount> BuildTable()
    {
        std::array<ValueCodec, EngineDataTypeCount> table{};

        auto set = [&](EngineDataType type, ValueCodec codec)
        {
            table[static_cast<size_t>(type)] = std::move(codec);
        };

        set(EngineDataType::Bool, MakeScalarCodec<bool, bool>());
        set(EngineDataType::Double, MakeScalarCodec<double, double>());
        set(EngineDataType::String, MakeScalarCodec<std::string, std::string>());

        set(EngineDataType::Int, MakeScalarCodec<int32_t, int64_t>());
        set(EngineDataType::Int2, MakeIntVecCodec<2>());
        set(EngineDataType::Int3, MakeIntVecCodec<3>());
        set(EngineDataType::Int4, MakeIntVecCodec<4>());

        set(EngineDataType::Float, MakeScalarCodec<float, double>());
        set(EngineDataType::Float2, MakeFloatVecCodec<2>());
        set(EngineDataType::Float3, MakeFloatVecCodec<3>());
        set(EngineDataType::Float4, MakeFloatVecCodec<4>());

        set(EngineDataType::Mat3, MakeGlmVecCodec<glm::mat3, float, double, 9, EngineDataType::Float>());
        set(EngineDataType::Mat4, MakeGlmVecCodec<glm::mat4, float, double, 16, EngineDataType::Float>());

        set(EngineDataType::GUID, MakeGuidCoded());
        set(EngineDataType::Enum, MakeEnumCodec());
        set(EngineDataType::ObjectPtr, MakeObjectPtrCodec());

        return table;
    }

    const ValueCodec *ValueCodecRegistry::Find(EngineDataType type)
    {
        static const auto table = BuildTable();

        const size_t index = static_cast<size_t>(type);
        if (index >= table.size() || !table[index].Capture)
        {
            CoreLogger::Error("No ValueCodec registered for EngineDataType {}", static_cast<int>(type));
            return nullptr;
        }

        return &table[index];
    }
}
