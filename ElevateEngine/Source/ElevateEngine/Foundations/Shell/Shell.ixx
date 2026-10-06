module;

#include <string_view>

export module Elevate.Foundations.Shell;

export namespace Elevate::Files
{
	void OpenWithDefaultApp(std::string_view filePath);
}