#pragma once

#include <memory>
#include <entt/entt.hpp>

#include <ElevateEngine/Core/GameContext.h>
#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Renderer/Cubemap.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

#include <ElevateEngine/Serialization/ISerializable.h>

// Forward declarations
namespace Elevate {
	class Cubemap;
	class Shader;
	class GameObject;
	class Camera;
}

namespace Elevate
{
	class ComponentRegistry;
	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;

	enum SceneType {
		RuntimeScene,
		EditorScene,
		DebugScene,
	};

	class Scene : public ISerializable
	{
	public:
		Scene();
		Scene(std::string name, SceneType type = SceneType::RuntimeScene);
		//~Scene();
		~Scene() = default;

		void UpdateScene();
		void RenderScene(Camera* cam = nullptr);
		void Notify(Event& event); // Dispatch an event to gameobjects

		inline const std::string& GetName() const { return m_name; };

		void AddObject(std::shared_ptr<GameObject> newObject, std::shared_ptr<GameObject> parent);
		const std::set<std::shared_ptr<GameObject>> GetRootObjects() const { return m_rootObjects; }

		inline SceneType GetType() { return m_type; }

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

		std::string Serialize() const override;
	private:
		void RemoveFromRoot(std::shared_ptr<GameObject> object);
		void AddRootObject(std::shared_ptr<GameObject> newRootObject);
	private:
		static uint32_t s_nextRegistryId;
	private:
		std::string m_name;
		SceneType m_type;

		uint32_t m_registryId; // Component registry id for all entt::entity

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT OU EXTENDRE LA CLASSE GAMEOBJECT
		std::set<std::shared_ptr<GameObject>> m_rootObjects;

		std::shared_ptr<Cubemap> m_cubemap;
		std::unique_ptr<SceneLighting> m_sceneLighting = nullptr;

		friend class GameObject;
		friend class ComponentRegistry;
	};
}