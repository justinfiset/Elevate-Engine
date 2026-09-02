#include "Profiler.h"

#include <chrono>
#include <thread>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Profiling/ProfilerSample.h>
#include <ElevateEngine/Core/Profiling/ProfilerFrame.h>

namespace Elevate
{
    uint64_t Profiler::m_FrameNumber = 0;
    ProfilerFrame Profiler::m_CurrentFrame = {};
    std::stack<ProfilerSample> Profiler::m_SampleStack;
    std::vector<ProfilerFrame> Profiler::m_Frames;

    void Profiler::BeginFrame()
    {
        m_FrameNumber++;
        m_SampleStack = std::stack<ProfilerSample>();

        m_CurrentFrame = ProfilerFrame();
        m_CurrentFrame.FrameNumber = m_FrameNumber;
    }

    void Profiler::EndFrame()
    {
        m_CurrentFrame.totalTimeMilliseconds = 0.0f;
        for (const auto& sample : m_CurrentFrame.Samples)
        {
            m_CurrentFrame.totalTimeMilliseconds += sample.GetDurationMilliseconds();
        }

        m_Frames.push_back(m_CurrentFrame);

        if (m_Frames.size() > MAX_FRAME_HISTORY)
        {
            m_Frames.erase(m_Frames.begin());
        }
    }

    void Profiler::BeginSample(const char* name)
    {
        ProfilerSample sample;
        sample.Name = name;
        sample.ThreadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
        sample.StartTime = std::chrono::high_resolution_clock::now();
        m_SampleStack.push(sample);
    }

    void Profiler::EndSample()
    {
        if (m_SampleStack.empty())
        {
            EE_CORE_ERROR("Profiler::EndSample() called without a matching BeginSample()");
            return; // Nothing left to close.
        }

        // Pop sample and add to the current frame
        ProfilerSample sample = m_SampleStack.top();
        m_SampleStack.pop();
        sample.EndTime = std::chrono::high_resolution_clock::now();
        m_CurrentFrame.Samples.push_back(sample);
    }
}