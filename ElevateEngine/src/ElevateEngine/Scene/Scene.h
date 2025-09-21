#pragma once

#include <entt/entt.hpp>
#include <ElevateEngine/Events/Event.h>

#include <ElevateEngine/Renderer/Cubemap.h>
#include <ElevateEngine/Core/GameContext.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>
#include <memory>

// Forward declarations
namespace Elevate {
	class Cubemap;
	class Shader;
	class GameObject;
	class Camera;
}

namespace Elevate
{
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
		Scene();
		Scene(std::string name, SceneType type = SceneType::RuntimeScene) 
			: m_Name(name), m_Type(type) {}

		~Scene() = default;
		
		void UpdateScene();
		void RenderScene(Camera* cam);
		void Notify(Event& event); // Dispatch an event to gameobjects

		inline const std::string& GetName() const { return m_Name; };

		void AddObject(std::shared_ptr<GameObject> newObject, std::shared_ptr<GameObject> parent);
		const std::set<std::shared_ptr<GameObject>> GetRootObjects() const { return m_rootObjects; }

		inline SceneType GetType() { return m_Type; }

		static ScenePtr Create(std::string name, SceneType type = SceneType::RuntimeScene);
			
		// Cubemap
		void SetSkybox(const char* skyboxFilePath);
		std::weak_ptr<Cubemap> GetSkybox();

		inline void SetLighting(std::unique_ptr<SceneLighting> newLighting)
		{
			m_sceneLighting = std::move(newLighting);
		}
		inline SceneLighting* GetSceneLighting() 
		{
			return m_sceneLighting.get();
		}
	private:
		void RemoveFromRoot(std::shared_ptr<GameObject> object);
		void AddRootObject(std::shared_ptr<GameObject> newRootObject);
	private:
		std::string m_Name;

		entt::registry m_Registry; // Component registry for all entt::entity

		std::shared_ptr<Cubemap> m_cubemap;

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT OU EXTENDRE LA CLASSE GAMEOBJECT
		std::set<std::shared_ptr<GameObject>> m_rootObjects;
		SceneType m_Type;

		std::unique_ptr<SceneLighting> m_sceneLighting = nullptr;

		friend class GameObject;
	};
}