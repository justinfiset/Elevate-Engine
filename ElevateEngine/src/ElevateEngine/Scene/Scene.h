#pragma once

#include <memory>
#include <entt/entt.hpp>
#include <ElevateEngine/Events/Event.h>

namespace Elevate
{
	class GameObject;
	// TODO SET ELSEWHERE
	using GameObjectPtr = std::shared_ptr<GameObject>;

	enum SceneType {
		RuntimeScene,
		EditorScene,
		DebugScene,
	};

	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;

	class Scene
	{
	public:
		Scene() = default;
		Scene(std::string name, SceneType type = SceneType::RuntimeScene) 
			: m_Name(name), m_Type(type) {}

		~Scene() = default;
		
		void UpdateScene();
		void RenderScene();
		void Notify(Event& event); // Dispatch an event to gameobjects

		inline const std::string GetName() const { return m_Name; };

		void AddObject(GameObjectPtr newObject, GameObjectPtr parent);
		const std::set<GameObjectPtr> GetRootObjects() const { return m_RootObjects; }

		inline SceneType GetType() { return m_Type; }

		static ScenePtr Create(std::string name, SceneType type = SceneType::RuntimeScene);

	private:
		void RemoveFromRoot(GameObjectPtr object);
		void AddRootObject(GameObjectPtr newRootObject);

	private:
		std::string m_Name;

		entt::registry m_Registry; // Component registry for all entt::entity

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT
		std::set<GameObjectPtr> m_RootObjects;
		SceneType m_Type;

		friend class GameObject;
	};
}