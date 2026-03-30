#include <ElevateEngine/Core/Platform.h>

#ifdef EE_PLATFORM_WEB
#include <emscripten.h>

namespace Elevate::Platform
{
    void OpenURL(const std::string& url)
    {
        // JS code to open a new tab with the specified URL
        EM_ASM({
            var url = UTF8ToString($0);
            window.open(url, '_blank');
        }, url.c_str());
    }
}

#endif