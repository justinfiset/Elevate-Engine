module;

#include <algorithm>
#include <memory>
#include <vector>

export module Elevate.Core.Scene.SceneManager;

import Elevate.Core.Scene:Scene;
import Elevate.Core.Scene:SceneType;
import Elevate.Core.Scene:ComponentRegistry;

export namespace Elevate
{
	class Scene;

	class SceneManager
	{
	public:
		static inline void LoadScene(std::shared_ptr<Scene> scene)
		{
			PushScene(scene);
		}

		static inline void UnloadScene(std::shared_ptr<Scene> scene)
		{
			PopScene(scene);
		}

		static ComponentRegistry* GetComponentRegistry()
		{
			return s_ComponentRegistry.get();
		}

		static inline void SetScene(std::shared_ptr<Scene> scene)
		{
			s_Scenes.clear();
			LoadScene(scene);
		}

		static inline std::shared_ptr<Scene> GetCurrentScene()
		{
			return s_Scenes.empty() ? nullptr : s_Scenes.back();
		}

		static inline std::shared_ptr<Scene> GetCurrentScene(SceneType type)
		{
			for (auto it = s_Scenes.rbegin(); it != s_Scenes.rend(); ++it)
			{
				if ((*it)->GetType() == type)
				{
					return *it;
				}
			}

			return nullptr;
		}

		static inline std::vector<std::shared_ptr<Scene>>::iterator begin()
		{
			return s_Scenes.begin();
		}

		static inline std::vector<std::shared_ptr<Scene>>::iterator end()
		{
			return s_Scenes.end();
		}

	private:
		static inline void PushScene(std::shared_ptr<Scene> scene)
		{
			s_Scenes.push_back(scene);
		}

		static inline void PopScene(std::shared_ptr<Scene> scene)
		{
			s_Scenes.erase(std::remove(s_Scenes.begin(), s_Scenes.end(), scene), s_Scenes.end());
		}

	private:
		static inline std::vector<std::shared_ptr<Scene>> s_Scenes;
		static inline std::unique_ptr<ComponentRegistry> s_ComponentRegistry = std::make_unique<ComponentRegistry>();
	};
}
