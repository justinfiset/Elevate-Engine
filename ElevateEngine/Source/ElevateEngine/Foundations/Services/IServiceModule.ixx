module;


export module Elevate.Foundations.Services:IServiceModule;

import :ServiceCollection;
import :ServiceProvider;

export namespace Elevate
{
	class IServiceModule
	{
	public:
		virtual ~IServiceModule() = default;

		virtual void Register(ServiceCollection& services) = 0;

		virtual void Configure(ServiceProvider& provider)
		{
			// Overriden by custom modules after all modules are registered
		}

		virtual void Shutdown()
		{
			// Overriden by custom modules before all modules are deleted.
			// This should be used to cleanup and break circular deps.
		}
	};
}
