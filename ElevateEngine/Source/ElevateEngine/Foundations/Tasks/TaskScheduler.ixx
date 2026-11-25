module;

#include <algorithm>
#include <array>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <mutex>
#include <span>
#include <thread>
#include <utility>
#include <vector>

export module Elevate.Foundations.Tasks.TaskScheduler;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Tasks.ITaskScheduler;
import Elevate.Foundations.Tasks.Task;
import Elevate.Foundations.Tasks.TaskAffinity;
import Elevate.Foundations.Tasks.TaskDescriptor;
import Elevate.Foundations.Tasks.TaskFunction;
import Elevate.Foundations.Tasks.TaskHandle;
import Elevate.Foundations.Tasks.TaskPool;
import Elevate.Foundations.Tasks.TaskQueue;

export namespace Elevate
{
	class TaskScheduler : public ITaskScheduler
	{
	public:
		explicit TaskScheduler(std::uint32_t workerCount = 0)
		{
			if (workerCount == 0)
			{
				const std::uint32_t hw = std::thread::hardware_concurrency();
				workerCount = hw > 3 ? hw - 2 : 1; // leave room for the main and IO threads
			}

			for (std::uint32_t i = 0; i < workerCount; ++i)
			{
				m_workers.emplace_back(
					[this]
					{
						WorkerLoop(TaskAffinity::Any);
					});
			}

			m_ioThread = std::thread(
				[this]
				{
					WorkerLoop(TaskAffinity::IOThread);
				});
		}

		~TaskScheduler()
		{
			{
				std::lock_guard lock(m_mutex);
				m_isDisposing = true;
			}

			m_condition.notify_all();

			// Wait for all worker threads to finish
			for (auto& worker : m_workers)
			{
				worker.join();
			}

			m_ioThread.join();
		}

		TaskScheduler(const TaskScheduler&) = delete;
		TaskScheduler& operator=(const TaskScheduler&) = delete;

		TaskHandle Submit(TaskDescriptor descriptor,
			TaskFunction function,
			std::span<const TaskHandle> dependsOn = {})
		{
			std::lock_guard lock(m_mutex);

			const TaskHandle handle = m_taskPool.Allocate(std::move(descriptor), std::move(function));
			Task* task = m_taskPool.Resolve(handle);

			for (const TaskHandle& dependencyHandle : dependsOn)
			{
				if (Task* dependency = m_taskPool.Resolve(dependencyHandle))
				{
					dependency->Dependents.push_back(handle);
					task->PendingDependencies.fetch_add(1, std::memory_order_relaxed);
				}
			}

			if (task->PendingDependencies.load(std::memory_order_relaxed) == 0)
			{
				Enqueue(handle);
			}

			return handle;
		}

		bool IsComplete(TaskHandle handle) const
		{
			std::lock_guard lock(m_mutex);
			return m_taskPool.Resolve(handle) == nullptr;
		}

		void Wait(TaskHandle handle)
		{
			std::unique_lock lock(m_mutex);
			m_condition.wait(lock, [&]
				{ return m_taskPool.Resolve(handle) == nullptr; });
		}

		void Update()
		{
			for (;;)
			{
				TaskHandle handle;
				{
					std::lock_guard lock(m_mutex);
					if (!Queue(TaskAffinity::MainThread).TryPop(handle))
					{
						return;
					}
				}

				Run(handle);
			}
		}

	private:
		TaskQueue& Queue(TaskAffinity affinity)
		{
			return m_queues[static_cast<std::size_t>(affinity)];
		}

		void Enqueue(TaskHandle handle)
		{
			Task* task = m_taskPool.Resolve(handle);

			if (!task)
			{
				CoreLogger::Warn("Attempted to enqueue an invalid task handle.");
				return;
			}

			Queue(task->Descriptor.Affinity)
				.Push(handle, task->Descriptor.Priority);

			m_condition.notify_all();
		}

		void Run(TaskHandle handle)
		{
			// Get the function
			TaskFunction function;
			{
				std::lock_guard lock(m_mutex);
				function = std::move(m_taskPool.Resolve(handle)->Function);
			}

			try
			{
				function();
			}
			catch (const std::exception& e)
			{
				CoreLogger::Error("Task threw an exception: {}", e.what());
			}
			catch (...)
			{
				CoreLogger::Error("Task threw an unknown exception.");
			}

			std::lock_guard lock(m_mutex);
			Complete(handle);
		}

		void Complete(TaskHandle handle)
		{
			Task* task = m_taskPool.Resolve(handle);

			for (const TaskHandle dependentHandle : task->Dependents)
			{
				Task* dependent = m_taskPool.Resolve(dependentHandle);
				if (dependent && dependent->PendingDependencies.fetch_sub(1, std::memory_order_acq_rel) == 1)
				{
					Enqueue(dependentHandle);
				}
			}

			m_taskPool.Release(handle);
			m_condition.notify_all();
		}

		void WorkerLoop(TaskAffinity affinity)
		{
			TaskQueue& queue = Queue(affinity);

			for (;;)
			{
				TaskHandle handle;
				{
					std::unique_lock lock(m_mutex);
					m_condition.wait(lock, [&]
						{ return m_isDisposing || !queue.IsEmpty(); });

					if (m_isDisposing)
					{
						return;
					}

					queue.TryPop(handle);
				}

				Run(handle);
			}
		}

		mutable std::mutex m_mutex;
		mutable std::condition_variable m_condition;

		mutable TaskPool m_taskPool;
		mutable std::array<TaskQueue, static_cast<std::size_t>(TaskAffinity::Count)> m_queues;

		bool m_isDisposing = false;

		std::vector<std::thread> m_workers;
		std::thread m_ioThread;
	};
}
