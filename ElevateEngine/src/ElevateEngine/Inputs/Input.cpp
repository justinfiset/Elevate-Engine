#include "Input.h"

#if defined(EE_PLATFORM_WINDOWS)
    #include "Platform/Windows/WindowsInput.h"
#elif defined(EE_PLATFORM_WEB)
    #include "Platform/Web/WebInput.h"
#elif defined(EE_PLATFORM_LINUX)
    #include "Platform/Linux/LinuxInput.h"
#endif

namespace Elevate
{
#if defined(EE_PLATFORM_WINDOWS) 
    using PlatformInput = WindowsInput;
#elif defined(EE_PLATFORM_LINUX)
    using PlatformInput = LinuxInput;
#elif defined(EE_PLATFORM_WEB)
    using PlatformInput = WebInput;
#else
    #error "Unknown platform! Cannot create input manager definition on this platform."
#endif

    Input* Input::s_Instance = new PlatformInput();
}