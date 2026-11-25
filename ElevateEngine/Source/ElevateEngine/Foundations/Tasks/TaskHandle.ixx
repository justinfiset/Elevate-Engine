module;

#include <cstdint>

export module Elevate.Foundations.Tasks.TaskHandle;

export namespace Elevate
{
    struct TaskHandle
    {
        std::uint32_t Index;
        std::uint32_t Generation;

        bool IsValid() const { return Generation != 0; }
        bool operator==(const TaskHandle &) const = default;
    };
}
