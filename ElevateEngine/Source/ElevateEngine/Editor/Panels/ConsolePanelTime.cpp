#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

#include <ElevateEngine/Editor/Panels/ConsolePanelTime.h>

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>

// Kept outside modules: MSVC 14.52 mis-compiles fmt's chrono formatter inside
// a module interface unit (C2752 on is_back_insert_iterator).
std::string Elevate::Editor::FormatConsoleTimestamp(std::chrono::system_clock::time_point timestamp)
{
	return fmt::format("{:%H:%M:%S}", timestamp);
}
