#include <cstdint>
#include <vector>
#include <span>

namespace Elevate
{
    using Byte = std::byte;
    using ByteBuffer = std::vector<Byte>;
    using ByteSpan = std::span<const Byte>;
}