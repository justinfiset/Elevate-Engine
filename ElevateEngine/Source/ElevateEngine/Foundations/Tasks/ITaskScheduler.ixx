module;

#include <cstdint>
#include <span>

export module Elevate.Foundations.Tasks.ITaskScheduler;

import Elevate.Foundations.Tasks.TaskDescriptor;
import Elevate.Foundations.Tasks.TaskFunction;
import Elevate.Foundations.Tasks.TaskHandle;

export namespace Elevate
{
    class ITaskScheduler
    {
    public:
        virtual ~ITaskScheduler() = default;

        virtual TaskHandle Submit(
            TaskDescriptor descriptor,
            TaskFunction function,
            std::span<const TaskHandle> dependsOn = {}) = 0;

        virtual bool IsComplete(TaskHandle handle) const = 0;

        virtual void Wait(TaskHandle handle) = 0;

        virtual void Update() = 0;
    };
}