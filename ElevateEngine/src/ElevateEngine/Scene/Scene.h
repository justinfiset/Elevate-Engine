#pragma once

#include <memory>
#include <entt/entt.hpp>
#include <ElevateEngine/Events/Event.h>

#include <ElevateEngine/Renderer/Cubemap.h>

namespace Elevate
{
	class GameObject;
	class Camera;
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
		void RenderScene(Camera* cam);
		void Notify(Event& event); // Dispatch an event to gameobjects

		inline const std::string GetName() const { return m_Name; };

		void AddObject(GameObjectPtr newObject, GameObjectPtr parent);
		const std::set<GameObjectPtr> GetRootObjects() const { return m_rootObjects; }

		inline SceneType GetType() { return m_Type; }

		static ScenePtr Create(std::string name, SceneType type = SceneType::RuntimeScene);

		// Cubemap
		void SetSkybox(Cubemap* cubemap);
		std::weak_ptr<Cubemap> GetSkybox();
	private:
		void RemoveFromRoot(GameObjectPtr object);
		void AddRootObject(GameObjectPtr newRootObject);

	private:
		std::string m_Name;

		entt::registry m_Registry; // Component registry for all entt::entity

		//std::unique_ptr<Cubemap> m_cubemap;
		std::shared_ptr<Cubemap> m_cubemap;

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT OU EXTENDRE LA CLASSE GAMEOBJECT
		std::set<GameObjectPtr> m_rootObjects;
		SceneType m_Type;

		friend class GameObject;
	};
}