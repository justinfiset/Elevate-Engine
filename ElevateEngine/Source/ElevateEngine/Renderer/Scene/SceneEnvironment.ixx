module;

#include <string>
#include <memory>

export module Elevate.Renderer.Scene.Environment;

// Foundations
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Cubemap;

export namespace Elevate
{
	struct SceneEnvironment
	{
		std::shared_ptr<Cubemap> cubemap;

		void SetCubemap(std::string cubemapFilePath)
		{
			cubemap.reset(Cubemap::CreateFromFile(cubemapFilePath));
		}
	};
}

export namespace Elevate
{
	SceneEnvironment* GetSceneEnvironment(Scene* scene)
	{
		if (!scene)
		{
			CoreLogger::Warn("GetSceneEnvironment: Scene is null.");
			return nullptr;
		}

		return scene->TryGetContext<SceneEnvironment>();
	}
}
