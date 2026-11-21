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
		virtual void Configure(ServiceProvider& provider) {}
		virtual void Shutdown() {}
	};
}
