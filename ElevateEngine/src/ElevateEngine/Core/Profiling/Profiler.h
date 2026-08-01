#pragma once
#include <cstdint>
#include <stack>

#include "ProfilerSample.h"
#include "ProfilerFrame.h"

namespace Elevate
{
    /**
     * @brief Simple profiler class that allows you to profile code execution time.
     * It uses a stack-based approach to allow for nested profiling samples. 
     * Each frame, you can call BeginFrame() and EndFrame() to mark the start and end of a frame,
     * and within that frame, you can call BeginSample() and EndSample() to mark the start and end of a profiling sample.
     * The profiler will automatically calculate the duration of each sample and store it in a ProfilerSample struct, which is then 
     * stored in a ProfilerFrame struct. The profiler also keeps track of the total time spent in each frame and stores it in the ProfilerFrame struct. 
     * The profiler maintains a history of the last MAX_FRAME_HISTORY frames, which can be accessed for analysis.
     * 
     */
    class Profiler
    {
    private:
        static constexpr size_t MAX_FRAME_HISTORY = 300;

    public:
        static void BeginFrame();
        static void EndFrame();

        static void BeginSample(const char *name);
        static void EndSample();

    private:
        static uint64_t m_FrameNumber;
        static ProfilerFrame m_CurrentFrame;
        static std::stack<ProfilerSample> m_SampleStack;
        static std::vector<ProfilerFrame> m_Frames;
    };

    /**
     * @brief Simple structure used to automatically profile a scope. When the object is created, 
     *  it will call Profiler::BeginSample() and when it goes out of scope, it will call Profiler::EndSample().
     */
    struct ProfileTimer
    {
        ProfileTimer(const char *name) { Profiler::BeginSample(name); }
        ~ProfileTimer() { Profiler::EndSample(); }
    };
}

#if defined(__GNUC__) || defined(__clang__)
    #define EE_PROFILER_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define EE_PROFILER_FUNC_SIG __FUNCSIG__
#else
    #define EE_PROFILER_FUNC_SIG __func__ // C++ standard fallback, not as descriptive as the others
#endif

#define EE_PROFILE_SCOPE(name) ::Elevate::ProfileTimer timer##__LINE__(name)
#define EE_PROFILE_FUNCTION() EE_PROFILE_SCOPE(EE_PROFILER_FUNC_SIG)