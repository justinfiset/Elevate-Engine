module;

#include <algorithm>
#include <memory>
#include <vector>
#include <string>

export module Elevate.Core.Scene:SceneManager;

import :Fwd;
import :Scene;
import :SceneType;
import :ComponentRegistry;

export namespace Elevate
{
	class SceneManager
	{
	public:
		SceneManager(std::shared_ptr<ComponentRegistry> componentRegistry)
			: m_componentRegistry(componentRegistry)
		{
		}

		std::shared_ptr<Scene> CreateScene(std::string name, SceneType type = SceneType::RuntimeScene)
		{
			auto scene = std::make_shared<Scene>(this, std::move(name), type);
			LoadScene(scene);
			return scene;
		}

		void LoadScene(std::shared_ptr<Scene> scene)
		{
			m_scenes.push_back(scene);
		}

		void UnloadScene(std::shared_ptr<Scene> scene)
		{
			m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene), m_scenes.end());
		}

		ComponentRegistry& GetComponentRegistry()
		{
			return *m_componentRegistry;
		}

		inline void SetScene(std::shared_ptr<Scene> scene)
		{
			m_scenes.clear();
			LoadScene(scene);
		}

		std::shared_ptr<Scene> GetCurrentScene()
		{
			return m_scenes.empty() ? nullptr : m_scenes.back();
		}

		std::shared_ptr<Scene> GetCurrentScene(SceneType type)
		{
			for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it)
			{
				if ((*it)->GetType() == type)
				{
					return *it;
				}
			}

			return nullptr;
		}

		auto begin() { return m_scenes.begin(); }
		auto end() { return m_scenes.end(); }
		auto begin() const { return m_scenes.begin(); }
		auto end() const { return m_scenes.end(); }

	private:
		std::shared_ptr<ComponentRegistry> m_componentRegistry;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
