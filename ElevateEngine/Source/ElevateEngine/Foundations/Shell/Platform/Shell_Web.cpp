module;

#ifdef EE_PLATFORM_WEB
#include <string>
#include <emscripten.h>
#endif

module Elevate.Foundations.Shell;
import :Native;

#ifdef EE_PLATFORM_WEB
namespace Elevate::Shell::Native
{
    bool Open(const std::string &target)
    {
        EM_ASM({ window.open(UTF8ToString($0), '_blank'); }, target.c_str());
        return true;
    }
}
#endif
