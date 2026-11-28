#pragma once

#include <chrono>
#include <string>

namespace Elevate::Editor
{
	std::string FormatConsoleTimestamp(std::chrono::system_clock::time_point timestamp);
}
