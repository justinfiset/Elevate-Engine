#include <ElevateEngine/Core/Platform.h>

#ifdef EE_PLATFORM_WINDOWS

#include <Windows.h>
#include <shellapi.h>

namespace Elevate::Platform
{
    void OpenURL(const std::string& url)
    {
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}

#endif