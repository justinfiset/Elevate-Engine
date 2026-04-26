#pragma once

#include <ElevateEngine/Serialization/ObjectPropertyField.h>
#include <ElevateEngine/Core/Byte.h>

namespace Elevate
{
    class Serializer
    {
    public:
        virtual ~Serializer() = default;

        /**
         * @brief Serializes properties into a binary buffer.
         * @param fields The properties to convert.
         * @param outBuffer The destination buffer for serialized data.
         * @return True if serialization succeeded, false otherwise.
         */
        virtual bool Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const = 0;
        /**
         * @brief Deserializes a binary buffer back into properties.
         * @param data The source buffer containing raw data.
         * @param outFields The property set to be populated.
         * @return True if data was valid and successfully mapped, false if corrupted or incompatible.
         */
        virtual bool Deserialize(const ByteBuffer& data, PropertySet& outFields) = 0;
    };
}