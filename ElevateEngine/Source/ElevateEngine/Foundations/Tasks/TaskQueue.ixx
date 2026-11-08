module;

#include <array>
#include <cstddef>
#include <deque>

export module Elevate.Foundations.Tasks.TaskQueue;

import Elevate.Foundations.Tasks.TaskPriority;
import Elevate.Foundations.Tasks.TaskHandle;

export namespace Elevate
{
	class TaskQueue
	{
	public:
		TaskQueue() = default;
		~TaskQueue() = default;

		void Push(TaskHandle handle, TaskPriority priority)
		{
			m_queues[static_cast<std::size_t>(priority)].push_back(handle);
		}

		bool TryPop(TaskHandle& outHandle)
		{
			for (std::size_t i = static_cast<std::size_t>(TaskPriority::Count); i-- > 0;)
			{
				auto& queue = m_queues[i];
				if (!queue.empty())
				{
					outHandle = queue.front();
					queue.pop_front();
					return true;
				}
			}

			return false;
		}

		bool IsEmpty() const
		{
			for (const auto& queue : m_queues)
			{
				if (!queue.empty())
				{
					return false;
				}
			}

			return true;
		}

	private:
		std::array<std::deque<TaskHandle>, static_cast<std::size_t>(TaskPriority::Count)> m_queues;
	};
}
