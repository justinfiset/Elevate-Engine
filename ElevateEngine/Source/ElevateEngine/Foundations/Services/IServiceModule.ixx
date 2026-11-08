export module Elevate.Foundations.Services:IServiceModule;

import :ServiceCollection;

export namespace Elevate
{
	class IServiceModule
	{
	public:
		virtual ~IServiceModule() = default;

		virtual void Register(ServiceCollection& services) = 0;
	};
}
