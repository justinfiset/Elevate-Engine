module;


#include <memory>
#include <new>
#include <typeinfo>

export module Elevate.Foundations.DI;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
import Elevate.Foundations.Tasks.ITaskScheduler;
import Elevate.Foundations.Tasks.TaskScheduler;

export namespace Elevate
{
	class FoundationsServiceModule : public IServiceModule
	{
	public:
		void Register(ServiceCollection& services) override
		{
			services.RegisterSingleton<ITaskScheduler, TaskScheduler>();

			CoreLogger::Info("Foundations Service Module Registered.");
		}
	};
}
