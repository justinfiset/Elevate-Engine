#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

#include <ElevateEngine/Core/Byte.h>

namespace Elevate
{
    class Guid
    {
    public:
        static Guid New();
        static Guid FromString(const std::string& str);

        bool IsValid() const;
        std::string ToString() const;
        ByteBuffer ToBytes() const;

        bool operator==(const Guid& other) const;
        bool operator!=(const Guid& other) const;
        bool operator<(const Guid& other) const;
        std::size_t GetHash() const noexcept;
    private:
        std::byte m_bytes[16];
    };
}

namespace std
{
    template<>
    struct hash<Elevate::Guid>
    {
        std::size_t operator()(const Elevate::Guid& guid) const noexcept
        {
            return std::hash<uint64_t>{}(guid.GetHash());
        }
    };
}