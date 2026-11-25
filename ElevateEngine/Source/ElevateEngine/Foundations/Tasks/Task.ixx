module;

#include <atomic>
#include <vector>
#include <cstdint>

export module Elevate.Foundations.Tasks.Task;

import Elevate.Foundations.Tasks.TaskHandle;
import Elevate.Foundations.Tasks.TaskDescriptor;
import Elevate.Foundations.Tasks.TaskFunction;

export namespace Elevate
{
	struct Task
	{
		TaskDescriptor Descriptor;
		TaskFunction Function;

		std::atomic<std::uint32_t> PendingDependencies{ 0 };
		std::vector<TaskHandle> Dependents;

		Task(TaskDescriptor descriptor, TaskFunction function)
			: Descriptor(std::move(descriptor)), Function(std::move(function))
		{
		}
	};
}
