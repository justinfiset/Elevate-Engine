#pragma once

import Elevate.Foundations;

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
	#include <shellapi.h>
#endif

namespace Elevate::Files
{
	void OpenWithDefaultApp(const std::string& filePath) {
		if (std::filesystem::exists(filePath)) {
#ifdef _WIN32
			// Windows
			ShellExecuteA(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __APPLE__
			// macOS
			std::string command = "open " + filePath + " &";
			system(command.c_str());
#elif __linux__
			// Linux
			std::string command = "xdg-open " + filePath + " &";
			system(command.c_str());
#else
			CoreLogger::Error("Unsupported OS");
#endif
		}
		else {
			CoreLogger::Error("ERROR - Cannot open file '{0}', file not found.", filePath);
		}
	}
}