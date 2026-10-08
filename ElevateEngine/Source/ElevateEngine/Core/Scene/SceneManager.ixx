module;

#include <vector>
#include <memory>

export module Elevate.Core.Scene.SceneManager;

import Elevate.Core.Scene.Scene;
import Elevate.Core.Scene.SceneType;

export namespace Elevate
{
	class Scene;

	class SceneManager
	{
	public:
		static inline void LoadScene(std::shared_ptr<Scene> scene) { PushScene(scene); }
		static inline void UnloadScene(std::shared_ptr<Scene> scene) { PopScene(scene); }

		static inline void SetScene(std::shared_ptr<Scene> scene)
		{
			m_Scenes.clear();
			LoadScene(scene);
		}

		static inline std::shared_ptr<Scene> GetCurrentScene()
		{
			if (!m_Scenes.empty())
			{
				return m_Scenes.back();
			}
			else return nullptr;
		}

		static inline std::shared_ptr<Scene> GetCurrentScene(SceneType type)
		{
			for (auto it = m_Scenes.end() - 1; it >= m_Scenes.begin(); it--)
			{
				std::shared_ptr<Scene> ptr = *it;
				if (it->get()->GetType() == type)
				{
					return ptr;
				}
			}
			return nullptr;
		}

		static inline std::vector<std::shared_ptr<Scene>>::iterator begin()
		{
			return m_Scenes.begin();
		}

		static inline std::vector<std::shared_ptr<Scene>>::iterator end()
		{
			return m_Scenes.end();
		}

	private:
		static inline void PushScene(std::shared_ptr<Scene> scene)
		{
			m_Scenes.push_back(scene);
		}

		static inline void PopScene(std::shared_ptr<Scene> scene)
		{
			m_Scenes.erase(std::remove(m_Scenes.begin(), m_Scenes.end(), scene), m_Scenes.end());
		}

	private:
		static inline std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}