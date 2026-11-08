module;

#include <cstdint>
#include <string_view>

export module Elevate.Foundations.Tasks.TaskAffinity;

export namespace Elevate
{
	enum class TaskAffinity : std::uint8_t
	{
		Any = 0,
		MainThread = 1,
		IOThread = 2,

		Count = 3
	};

	constexpr std::string_view ToString(TaskAffinity a) noexcept
	{
		switch (a)
		{
		case TaskAffinity::Any:
			return "Any";
		case TaskAffinity::MainThread:
			return "MainThread";
		case TaskAffinity::IOThread:
			return "IOThread";
		default:
			return "Unknown";
		}
	}
}
