module;

#include <filesystem>
#include <string>

module Elevate.Foundations.Shell;

import :Native;
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Paths;

namespace Elevate::Shell
{
	void OpenWithDefaultApp(std::string_view filePath)
	{
		auto fsPath = PathResolver::Resolve(filePath);
		if (!std::filesystem::exists(fsPath))
		{
			CoreLogger::Error("ERROR - Cannot open file '{0}', file not found.", filePath);
			return;
		}
		if (!Native::Open(fsPath.string()))
		{
			CoreLogger::Error("Unsupported OS or failed to open '{0}'.", filePath);
		}
	}

	void OpenURL(std::string_view url)
	{
		if (!Native::Open(std::string(url)))
		{
			CoreLogger::Error("Failed to open URL '{0}'.", url);
		}
	}
}
