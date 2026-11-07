module;

#include <cstdint>
#include <vector>

export module Elevate.Core.Profiling:ProfilerFrame;

import :ProfilerSample;

export namespace Elevate
{
    struct ProfilerFrame
    {
        uint64_t FrameNumber;
        float totalTimeMilliseconds;
        std::vector<ProfilerSample> Samples;
    };
}
