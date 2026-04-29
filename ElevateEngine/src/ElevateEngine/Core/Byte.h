#pragma once

#include <cstdint>
#include <vector>
#include <span>
#include <string>
#include <sstream>
#include <iomanip>

namespace Elevate
{
    using Byte = std::byte;
    using ByteBuffer = std::vector<Byte>;
    using ByteSpan = std::span<const Byte>;
    
    /**
     * Namespace that contians function to ease the use of ByteBuffers and ByteSpans.
     */
    namespace ByteUitls
    {
        /**
         * Converts any combination of bytes to a string.
         * 
         * \param bytes Any type of byte array.
         * \return A string built from the content of the given byte array.
         */
        inline std::string ToString(const ByteSpan& bytes)
        {
            return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
        }

        /**
         * Converts any combination of bytes into a string in the hexadecimal format with
         * with the given chars range : [0,f].
         * 
         * \param bytes Any type of byte array.
         * \return A string built from the content of the given byte array.
         */
        inline std::string ToHexString(const ByteSpan& bytes)
        {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (auto byte : bytes)
            {
                ss << std::setw(2) << static_cast<int>(byte);
            }
            return ss.str();
        }
    }
}