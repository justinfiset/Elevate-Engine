module;

#include <atomic>
#include <vector>

export module Elevate.Foundations.Tasks.Task;
import Elevate.Foundations.Tasks.TaskDescriptor;
import Elevate.Foundations.Tasks.TaskFunction;

export namespace Elevate
{
	struct Task
	{
		TaskDescriptor Descriptor;
		TaskFunction Function;

		std::atomic<std::uint32_t> PendingDependencies{ 0 };
		std::vector<Task*> Dependents;
	};

	using TaskHandle = Task*;
}
