#pragma once

#include <memory>
#include <entt/entt.hpp>
#include <ElevateEngine/Events/Event.h>

namespace Elevate
{
	class GameObject;
	using GameObjectPtr = std::shared_ptr<GameObject>;

	class Scene
	{
	public:
		Scene();
		Scene(std::string name) : m_Name(name) {}
		~Scene();
		
		void UpdateScene();
		void RenderScene();
		void Notify(Event& event); // Dispatch an event to gameobjects

		inline const std::string GetName() const { return m_Name; };

		void AddObject(GameObjectPtr newObject, GameObjectPtr parent);
		const std::set<GameObjectPtr> GetRootObjects() const { return m_RootObjects; }
	private:
		void AddRootObject(GameObjectPtr newRootObject);
	private:
		std::string m_Name;

		entt::registry m_Registry; // Component registry for all entt::entity

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT
		std::set<GameObjectPtr> m_RootObjects;

		friend class GameObject;
	};

	using ScenePtr = std::shared_ptr<Scene>;
}