export module Elevate.Core.ServiceModule;

import Elevate.Foundations.Services;

import Elevate.Core.Scene;

export namespace Elevate
{
	class CoreServiceModule : public IServiceModule
	{
	public:
		void Register(ServiceCollection& services) override
		{
			services.RegisterSingleton<ComponentRegistry, ComponentRegistry>();
			services.RegisterSingleton<SceneManager, SceneManager, ComponentRegistry>();
		}
	};
}
