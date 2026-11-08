#include <catch2/catch_test_macros.hpp>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

import Elevate;

using namespace Elevate;
using namespace std::chrono_literals;

namespace
{
	TaskDescriptor MakeDescriptor(TaskAffinity affinity = TaskAffinity::Any, TaskPriority priority = TaskPriority::Normal)
	{
		TaskDescriptor d;
		d.Affinity = affinity;
		d.Priority = priority;
		return d;
	}

	bool WaitFor(TaskScheduler& scheduler, TaskHandle handle, std::chrono::milliseconds timeout = 5000ms)
	{
		const auto deadline = std::chrono::steady_clock::now() + timeout;
		while (!scheduler.IsComplete(handle))
		{
			scheduler.Update();
			if (std::chrono::steady_clock::now() > deadline)
			{
				return false;
			}
			std::this_thread::sleep_for(1ms);
		}
		return true;
	}

	class OrderLog
	{
	public:
		void Add(std::string entry)
		{
			std::lock_guard lock(m_mutex);
			m_entries.push_back(std::move(entry));
		}

		std::vector<std::string> Get() const
		{
			std::lock_guard lock(m_mutex);
			return m_entries;
		}

	private:
		mutable std::mutex m_mutex;
		std::vector<std::string> m_entries;
	};
}

// ---------------------------------------------------------------------------
// TaskQueue
// ---------------------------------------------------------------------------

TEST_CASE("TaskQueue pops higher priorities first", "[Tasks][Queue]")
{
	TaskQueue queue;
	queue.Push(TaskHandle{ 1, 1 }, TaskPriority::Low);
	queue.Push(TaskHandle{ 2, 1 }, TaskPriority::Normal);
	queue.Push(TaskHandle{ 3, 1 }, TaskPriority::High);

	TaskHandle out;
	REQUIRE(queue.TryPop(out));
	CHECK(out.Index == 3);
	REQUIRE(queue.TryPop(out));
	CHECK(out.Index == 2);
	REQUIRE(queue.TryPop(out));
	CHECK(out.Index == 1);
}

TEST_CASE("TaskQueue is FIFO within the same priority", "[Tasks][Queue]")
{
	TaskQueue queue;
	for (std::uint32_t i = 0; i < 5; ++i)
	{
		queue.Push(TaskHandle{ i, 1 }, TaskPriority::Normal);
	}

	for (std::uint32_t i = 0; i < 5; ++i)
	{
		TaskHandle out;
		REQUIRE(queue.TryPop(out));
		CHECK(out.Index == i);
	}
}

TEST_CASE("TaskQueue TryPop on an empty queue fails and leaves the output untouched", "[Tasks][Queue]")
{
	TaskQueue queue;
	TaskHandle out{ 42, 7 };

	CHECK(queue.IsEmpty());
	CHECK_FALSE(queue.TryPop(out));
	CHECK(out.Index == 42);
	CHECK(out.Generation == 7);
}

// ---------------------------------------------------------------------------
// TaskPool
// ---------------------------------------------------------------------------

TEST_CASE("TaskPool Allocate returns a resolvable handle", "[Tasks][Pool]")
{
	TaskPool pool;

	const TaskHandle handle = pool.Allocate(MakeDescriptor(), [] {});

	CHECK(handle.IsValid());
	CHECK(pool.Resolve(handle) != nullptr);
}

TEST_CASE("TaskPool Release invalidates the handle", "[Tasks][Pool]")
{
	TaskPool pool;
	const TaskHandle handle = pool.Allocate(MakeDescriptor(), [] {});

	pool.Release(handle);

	CHECK(pool.Resolve(handle) == nullptr);
}

TEST_CASE("TaskPool reuses a released slot with a new generation", "[Tasks][Pool]")
{
	TaskPool pool;
	const TaskHandle first = pool.Allocate(MakeDescriptor(), [] {});
	pool.Release(first);

	const TaskHandle second = pool.Allocate(MakeDescriptor(), [] {});

	CHECK(second.Index == first.Index);
	CHECK(second.Generation != first.Generation);
	CHECK(pool.Resolve(first) == nullptr);
	CHECK(pool.Resolve(second) != nullptr);
}

TEST_CASE("TaskPool keeps task addresses stable while growing", "[Tasks][Pool]")
{
	TaskPool pool;
	const TaskHandle first = pool.Allocate(MakeDescriptor(), [] {});
	Task* address = pool.Resolve(first);

	for (int i = 0; i < 10000; ++i)
	{
		pool.Allocate(MakeDescriptor(), [] {});
	}

	CHECK(pool.Resolve(first) == address);
}

TEST_CASE("TaskPool Resolve rejects an out of range handle", "[Tasks][Pool]")
{
	TaskPool pool;

	CHECK(pool.Resolve(TaskHandle{ 999, 1 }) == nullptr);
	CHECK(pool.Resolve(TaskHandle{}) == nullptr);
}

// ---------------------------------------------------------------------------
// TaskScheduler
// ---------------------------------------------------------------------------

TEST_CASE("TaskScheduler runs a simple task", "[Tasks][Scheduler]")
{
	TaskScheduler scheduler(2);
	std::atomic<bool> ran{ false };

	const TaskHandle handle = scheduler.Submit(MakeDescriptor(), [&]
		{ ran = true; });

	REQUIRE(WaitFor(scheduler, handle));
	CHECK(ran);
	CHECK(scheduler.IsComplete(handle));
}

TEST_CASE("TaskScheduler treats an invalid handle as complete", "[Tasks][Scheduler]")
{
	TaskScheduler scheduler(1);

	CHECK(scheduler.IsComplete(TaskHandle{}));
}

TEST_CASE("TaskScheduler runs Any tasks off the calling thread", "[Tasks][Scheduler][Threads]")
{
	TaskScheduler scheduler(2);
	std::thread::id taskThread;

	const TaskHandle handle = scheduler.Submit(MakeDescriptor(TaskAffinity::Any),
		[&]
		{ taskThread = std::this_thread::get_id(); });

	REQUIRE(WaitFor(scheduler, handle));
	CHECK(taskThread != std::this_thread::get_id());
}

TEST_CASE("TaskScheduler runs IOThread tasks off the calling thread", "[Tasks][Scheduler][Threads]")
{
	TaskScheduler scheduler(1);
	std::thread::id taskThread;

	const TaskHandle handle = scheduler.Submit(MakeDescriptor(TaskAffinity::IOThread),
		[&]
		{ taskThread = std::this_thread::get_id(); });

	REQUIRE(WaitFor(scheduler, handle));
	CHECK(taskThread != std::this_thread::get_id());
}

TEST_CASE("TaskScheduler runs MainThread tasks only inside Update", "[Tasks][Scheduler][Threads]")
{
	TaskScheduler scheduler(2);
	std::atomic<bool> ran{ false };
	std::thread::id taskThread;

	const TaskHandle handle = scheduler.Submit(MakeDescriptor(TaskAffinity::MainThread), [&]
		{
			ran = true;
			taskThread = std::this_thread::get_id(); });

	std::this_thread::sleep_for(50ms);
	CHECK_FALSE(ran);
	CHECK_FALSE(scheduler.IsComplete(handle));

	scheduler.Update();

	CHECK(ran);
	CHECK(taskThread == std::this_thread::get_id());
	CHECK(scheduler.IsComplete(handle));
}

TEST_CASE("TaskScheduler respects a dependency chain across affinities", "[Tasks][Scheduler][Dependencies]")
{
	TaskScheduler scheduler(2);
	OrderLog log;

	const TaskHandle read = scheduler.Submit(MakeDescriptor(TaskAffinity::IOThread), [&]
		{ log.Add("read"); });
	const TaskHandle parse = scheduler.Submit(MakeDescriptor(TaskAffinity::Any), [&]
		{ log.Add("parse"); }, std::array{ read });
	const TaskHandle upload = scheduler.Submit(MakeDescriptor(TaskAffinity::MainThread), [&]
		{ log.Add("upload"); }, std::array{ parse });

	REQUIRE(WaitFor(scheduler, upload));

	const std::vector<std::string> expected{ "read", "parse", "upload" };
	CHECK(log.Get() == expected);
}

TEST_CASE("TaskScheduler waits for every dependency (fan-in)", "[Tasks][Scheduler][Dependencies]")
{
	TaskScheduler scheduler(4);
	std::atomic<int> finished{ 0 };
	std::atomic<int> seenByJoin{ -1 };

	std::array<TaskHandle, 3> parents;
	for (auto& parent : parents)
	{
		parent = scheduler.Submit(MakeDescriptor(), [&]
			{
				std::this_thread::sleep_for(10ms);
				++finished; });
	}

	const TaskHandle join = scheduler.Submit(MakeDescriptor(), [&]
		{ seenByJoin = finished.load(); }, parents);

	REQUIRE(WaitFor(scheduler, join));
	CHECK(seenByJoin == 3);
}

TEST_CASE("TaskScheduler accepts a dependency that already completed", "[Tasks][Scheduler][Dependencies]")
{
	TaskScheduler scheduler(2);
	const TaskHandle first = scheduler.Submit(MakeDescriptor(), [] {});
	REQUIRE(WaitFor(scheduler, first));

	std::atomic<bool> ran{ false };
	const TaskHandle second = scheduler.Submit(MakeDescriptor(), [&]
		{ ran = true; }, std::array{ first });

	REQUIRE(WaitFor(scheduler, second));
	CHECK(ran);
}

TEST_CASE("TaskScheduler runs higher priority tasks first", "[Tasks][Scheduler][Priority]")
{
	TaskScheduler scheduler(1);
	OrderLog log;
	std::atomic<bool> gateStarted{ false };
	std::atomic<bool> gateRelease{ false };

	const TaskHandle gate = scheduler.Submit(MakeDescriptor(), [&]
		{
			gateStarted = true;
			while (!gateRelease)
			{
				std::this_thread::yield();
			} });

			while (!gateStarted)
			{
				std::this_thread::yield();
			}

			const TaskHandle low = scheduler.Submit(MakeDescriptor(TaskAffinity::Any, TaskPriority::Low), [&]
				{ log.Add("low"); });
			const TaskHandle normal = scheduler.Submit(MakeDescriptor(TaskAffinity::Any, TaskPriority::Normal), [&]
				{ log.Add("normal"); });
			const TaskHandle high = scheduler.Submit(MakeDescriptor(TaskAffinity::Any, TaskPriority::High), [&]
				{ log.Add("high"); });

			gateRelease = true;

			REQUIRE(WaitFor(scheduler, gate));
			REQUIRE(WaitFor(scheduler, low));
			REQUIRE(WaitFor(scheduler, normal));
			REQUIRE(WaitFor(scheduler, high));

			const std::vector<std::string> expected{ "high", "normal", "low" };
			CHECK(log.Get() == expected);
}

TEST_CASE("TaskScheduler still releases dependents when a task throws", "[Tasks][Scheduler][Errors]")
{
	TaskScheduler scheduler(2);
	std::atomic<bool> dependentRan{ false };

	const TaskHandle thrower = scheduler.Submit(MakeDescriptor(), []
		{ throw std::runtime_error("Boom. This is normal."); });
	const TaskHandle dependent = scheduler.Submit(MakeDescriptor(), [&]
		{ dependentRan = true; }, std::array{ thrower });

	REQUIRE(WaitFor(scheduler, dependent));
	CHECK(scheduler.IsComplete(thrower));
	CHECK(dependentRan);
}

TEST_CASE("TaskScheduler runs many independent tasks", "[Tasks][Scheduler][Stress]")
{
	constexpr int count = 10000;
	TaskScheduler scheduler(4);
	std::atomic<int> counter{ 0 };
	std::vector<TaskHandle> handles;
	handles.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		handles.push_back(scheduler.Submit(MakeDescriptor(), [&]
			{ ++counter; }));
	}

	for (const TaskHandle handle : handles)
	{
		REQUIRE(WaitFor(scheduler, handle));
	}

	CHECK(counter == count);
}

TEST_CASE("TaskScheduler can be destroyed with tasks that never became ready", "[Tasks][Scheduler][Shutdown]")
{
	{
		TaskScheduler scheduler(2);

		const TaskHandle neverRuns = scheduler.Submit(MakeDescriptor(TaskAffinity::MainThread), [] {});
		scheduler.Submit(MakeDescriptor(), [] {}, std::array{ neverRuns });
	}

	SUCCEED();
}