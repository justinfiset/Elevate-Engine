#if defined(__GNUC__) || defined(__clang__)
    #define EE_PROFILER_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define EE_PROFILER_FUNC_SIG __FUNCSIG__
#else
    #define EE_PROFILER_FUNC_SIG __func__ // C++ standard fallback, not as descriptive as the others
#endif

#define EE_PROFILE_SCOPE(name) ::Elevate::ProfileTimer timer##__LINE__(name)
#define EE_PROFILE_FUNCTION() EE_PROFILE_SCOPE(EE_PROFILER_FUNC_SIG)
