module;

#include <string>

#ifdef EE_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <shellapi.h>
#endif

module Elevate.Foundations.Shell;
import :Native;

#ifdef EE_PLATFORM_WINDOWS
namespace Elevate::Shell::Native
{
    bool Open(const std::string &target)
    {
        return reinterpret_cast<INT_PTR>(ShellExecuteA(nullptr, "open", target.c_str(), nullptr, nullptr, SW_SHOWNORMAL)) > 32;
    }
}
#endif
