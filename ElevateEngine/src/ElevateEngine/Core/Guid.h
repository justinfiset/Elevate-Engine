#pragma once

#include <string>
#include <ElevateEngine/Core/Byte.h>

namespace Elevate
{
    class Guid
    {
    public:
        static Guid New();

        std::string ToString() const;
        ByteBuffer ToBytes() const;

        bool operator==(const Guid& other) const;

    private:
        std::byte m_bytes[16];
    };
}