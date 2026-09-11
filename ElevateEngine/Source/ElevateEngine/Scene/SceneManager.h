#pragma once

#include <vector>

#include <ElevateEngine/Scene/Scene.h>

namespace Elevate
{
	class SceneManager {
	public:
		static inline void LoadScene(ScenePtr scene) { PushScene(scene); }
		static inline void UnloadScene(ScenePtr scene) { PopScene(scene); }

		static inline void SetScene(ScenePtr scene) 
		{ 
			m_Scenes.clear();
			LoadScene(scene);
		}

		static inline ScenePtr GetCurrentScene() 
		{
			if (!m_Scenes.empty())
			{
				return m_Scenes.back();
			}
			else return nullptr;
		}

		static inline ScenePtr GetCurrentScene(SceneType type) {
			for (auto it = m_Scenes.end() - 1; it >= m_Scenes.begin(); it--) {
				ScenePtr ptr = *it;
				if (it->get()->GetType() == type) {
					return ptr;
				}
			}
			return nullptr;
		}

		static inline std::vector<ScenePtr>::iterator begin() { return m_Scenes.begin(); }
		static inline std::vector<ScenePtr>::iterator end() { return m_Scenes.end(); }

	private:
		static inline void PushScene(ScenePtr scene) { m_Scenes.push_back(scene); }
		static inline void PopScene(ScenePtr scene) 
		{
			m_Scenes.erase(std::remove(m_Scenes.begin(), m_Scenes.end(), scene), m_Scenes.end());
		}

	private:
		static inline std::vector<ScenePtr> m_Scenes;
	};
}