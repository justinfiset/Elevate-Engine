export module Elevate.Foundations.Tasks.TaskDescriptor;

export import Elevate.Foundations.Tasks.TaskAffinity;
export import Elevate.Foundations.Tasks.TaskPriority;

export namespace Elevate
{
	struct TaskDescriptor
	{
		TaskAffinity Affinity;
		TaskPriority Priority;
		const char* Name;
	};
}
