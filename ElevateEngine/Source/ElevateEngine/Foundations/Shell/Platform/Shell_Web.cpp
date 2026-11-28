module;

#ifdef EE_PLATFORM_WEB
#include <string>
#include <emscripten.h>
#endif

module Elevate.Foundations.Shell;
import :Native;
import Elevate.Foundations.CoreLogger;

#ifdef EE_PLATFORM_WEB
namespace Elevate::Shell::Native
{
    bool Open(const std::string &target)
    {
        if (target.rfind("http://", 0) != 0 && target.rfind("https://", 0) != 0)
        {
            CoreLogger::Warn("Refusing to open non-HTTP target on Web: {}", target);
            return false;
        }

        EM_ASM({ window.open(UTF8ToString($0), '_blank'); }, target.c_str());
        return true;
    }
}
#endif
