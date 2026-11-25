module;

#include <string_view>

export module Elevate.Foundations.Shell;

export namespace Elevate::Shell
{
	void OpenWithDefaultApp(std::string_view filePath);
	void OpenURL(const std::string_view url);
}
