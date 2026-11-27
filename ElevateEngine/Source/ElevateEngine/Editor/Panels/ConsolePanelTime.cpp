#include <ElevateEngine/Editor/Panels/ConsolePanelTime.h>

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>

// Kept outside modules: MSVC mis-compiles fmt's chrono formatter inside a
// module interface unit.
std::string Elevate::Editor::FormatConsoleTimestamp(std::chrono::system_clock::time_point timestamp)
{
	return fmt::format("{:%H:%M:%S}", timestamp);
}
