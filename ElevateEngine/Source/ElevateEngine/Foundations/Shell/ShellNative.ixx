module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <string>

export module Elevate.Foundations.Shell:Native;

namespace Elevate::Shell::Native
{
	bool Open(const std::string& target);
}
