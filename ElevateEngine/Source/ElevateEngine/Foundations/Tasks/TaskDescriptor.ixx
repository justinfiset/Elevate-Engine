module;

#include <string>
#include <utility>

export module Elevate.Foundations.Tasks.TaskDescriptor;

export import Elevate.Foundations.Tasks.TaskAffinity;
export import Elevate.Foundations.Tasks.TaskPriority;

export namespace Elevate
{
	struct TaskDescriptor
	{
		TaskAffinity Affinity;
		TaskPriority Priority;
		std::string Name;

		TaskDescriptor(TaskAffinity affinity, TaskPriority priority, std::string name)
			: Affinity(affinity), Priority(priority), Name(std::move(name))
		{
		}

		TaskDescriptor() : TaskDescriptor(TaskAffinity::Any, TaskPriority::Normal, "Task")
		{
		}
	};
}
