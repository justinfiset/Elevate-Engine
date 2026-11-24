module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <functional>

export module Elevate.Foundations.Tasks.TaskFunction;

export namespace Elevate
{
	using TaskFunction = std::function<void()>;
}
