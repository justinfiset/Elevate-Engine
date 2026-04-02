#pragma once

#include <ElevateEngine/Serialization/ObjectPropertyField.h>
#include <ElevateEngine/Core/Byte.h>

namespace Elevate
{
    class Serializer
    {
    public:
        virtual ~Serializer() = default;

        virtual bool Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const = 0;
        virtual bool Deserialize(const ByteBuffer& data, PropertySet& outFields) = 0;
    }
}