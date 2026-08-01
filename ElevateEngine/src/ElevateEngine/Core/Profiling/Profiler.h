#pragma once
#include <cstdint>
#include <stack>

#include "ProfilerSample.h"
#include "ProfilerFrame.h"

namespace Elevate
{
    class Profiler
    {
    private:
        static constexpr size_t MAX_FRAME_HISTORY = 300;

    public:
        static void BeginFrame();
        static void EndFrame();

        static void BeginSample(const char* name);
        static void EndSample();

    private:
        static uint64_t m_FrameNumber;
        static ProfilerFrame m_CurrentFrame;
        static std::stack<ProfilerSample> m_SampleStack;
        static std::vector<ProfilerFrame> m_Frames;
    };
}