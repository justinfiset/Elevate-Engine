module;

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #include <shellapi.h>
#endif

#include <filesystem>

module Elevate.Foundations.Shell;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Paths;

namespace Elevate::Files
{
    void OpenWithDefaultApp(std::string_view filePath)
    {
        auto fsPath = PathResolver::Resolve(filePath);
        if (std::filesystem::exists(fsPath))
        {
#ifdef _WIN32
            // Windows
            ShellExecuteA(NULL, "open", fsPath.string().c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __APPLE__
            // macOS
            std::string command = "open " + filePath.string() + " &";
            system(command.c_str());
#elif __linux__
            // Linux
            std::string command = "xdg-open " + filePath.string() + " &";
            system(command.c_str());
#else
            CoreLogger::Error("Unsupported OS");
#endif
        }
        else
        {
            CoreLogger::Error("ERROR - Cannot open file '{0}', file not found.", filePath);
        }
    }
}