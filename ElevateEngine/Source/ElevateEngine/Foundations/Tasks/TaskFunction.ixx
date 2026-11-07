module;

#include <functional>

export module Elevate.Foundations.Tasks.TaskFunction;

export namespace Elevate
{
	using TaskFunction = std::function<void()>;
}
