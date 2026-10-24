module;

#include <memory>
#include <vector>

module Elevate.Core.Scene;

namespace Elevate
{
	std::vector<std::shared_ptr<Scene>> SceneManager::s_Scenes;
	std::unique_ptr<ComponentRegistry> SceneManager::s_ComponentRegistry = std::make_unique<ComponentRegistry>();
}
