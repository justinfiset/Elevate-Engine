module;

#include <cstdint>
#include <atomic>
#include <deque>
#include <vector>

export module Elevate.Foundations.Tasks.TaskPool;

import Elevate.Foundations.CoreLogger;

import Elevate.Foundations.Tasks.TaskHandle;
import Elevate.Foundations.Tasks.TaskDescriptor;
import Elevate.Foundations.Tasks.TaskFunction;
import Elevate.Foundations.Tasks.Task;

export namespace Elevate
{
	class TaskPool
	{
	public:
		TaskPool() = default;
		~TaskPool() = default;

		TaskHandle Allocate(TaskDescriptor descriptor, TaskFunction function)
		{
			std::uint32_t index;

			if (!m_freeSlots.empty())
			{
				index = m_freeSlots.back();
				m_freeSlots.pop_back();
			}
			else
			{
				index = static_cast<std::uint32_t>(m_slots.size());
				m_slots.emplace_back();
				m_generations.push_back(1);
			}

			Task& task = m_slots[index];
			task.Descriptor = descriptor;
			task.Function = function;
			task.PendingDependencies.store(0, std::memory_order_relaxed);
			task.Dependents.clear();

			return TaskHandle{ index, m_generations[index] };
		}

		Task* Resolve(TaskHandle handle)
		{
			if (handle.Index >= m_slots.size() || m_generations[handle.Index] != handle.Generation)
			{
				return nullptr;
			}

			return &m_slots[handle.Index];
		}

		void Release(TaskHandle handle)
		{
			Task* task = Resolve(handle);

			if (!task)
			{
				CoreLogger::Warn("Attempted to deallocate an invalid task handle.");
				return;
			}

			// Release the task slot
			task->Function = {};
			task->Descriptor = {};

			// Invalide the handle by incrementing the generation
			std::uint32_t& gen = m_generations[handle.Index];
			if (++gen == 0)
			{
				gen = 1; // Avoid generation wrap-around to 0
			}

			m_freeSlots.push_back(handle.Index);
		}

	private:
		std::deque<Task> m_slots;
		std::vector<std::uint32_t> m_generations;
		std::vector<std::uint32_t> m_freeSlots;
	};
}
