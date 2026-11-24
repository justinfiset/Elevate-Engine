module;

#include <ElevateEngine/Core/Scene/SceneEnTTOperators.h>

export module Elevate.Renderer.DI;

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
// Renderer
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Materials;

export namespace Elevate
{
	class RendererServiceModule : public IServiceModule
	{
	public:
		void Register(ServiceCollection& services) override
		{
			CoreLogger::Info("Renderer Service Module Registered.");
		}

		void Configure(ServiceProvider& serviceProvider) override
		{
			ShaderManager::Init();
			MaterialRegistry::Init();

			CoreLogger::Info("Renderer Service Module Configured.");
		}
	};
}
