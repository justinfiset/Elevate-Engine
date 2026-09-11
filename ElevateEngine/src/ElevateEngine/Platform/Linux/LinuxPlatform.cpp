#include <ElevateEngine/Core/Platform.h>

#ifdef EE_PLATFORM_LINUX

#include <cstdlib>
#include <string>

namespace Elevate::Platform
{
    void OpenURL(const std::string& url)
    {
        std::system(("xdg-open " + url + " &").c_str());
    }
}

#endif