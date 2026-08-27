#pragma once
#include <cstdint>
#include <vector>

#include "ProfilerSample.h"

namespace Elevate
{
    struct ProfilerFrame
    {
        uint64_t FrameNumber;
        float totalTimeMilliseconds;
        std::vector<ProfilerSample> Samples;
    };
}