module;

#include <cstdint>

export module Elevate.Foundations.Tasks.TaskPriority;

export namespace Elevate
{
    enum class TaskPriority : std::uint8_t
    {
        Low = 0,
        Normal,
        High,

        Count
    };
}
