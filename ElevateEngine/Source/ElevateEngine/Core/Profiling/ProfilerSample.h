#pragma once
#include <chrono>
#include <cstdint>

namespace Elevate
{
    struct ProfilerSample
    {
        const char* Name = nullptr;
        uint32_t ThreadID;

        std::chrono::high_resolution_clock::time_point StartTime;
        std::chrono::high_resolution_clock::time_point EndTime;

        [[nodiscard]] float GetDurationMilliseconds() const
        {
            return std::chrono::duration<float, std::milli>(EndTime - StartTime).count();
        }

        [[nodiscard]] uint64_t GetDurationMicroseconds() const
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime).count();
        }
    };
}