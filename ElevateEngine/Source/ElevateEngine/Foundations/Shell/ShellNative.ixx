module;

#include <string>

export module Elevate.Foundations.Shell:Native;

namespace Elevate::Shell::Native
{
	export bool Open(const std::string& target);
}
