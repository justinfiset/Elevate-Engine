module;

#include <functional>
#include <string_view>

export module Elevate.Foundations.Logger.LogCallback;

export import Elevate.Foundations.Logger.LogLevel;

export namespace Elevate
{
	using LogCallback = std::function<void(LogLevel, std::string_view)>;
}
