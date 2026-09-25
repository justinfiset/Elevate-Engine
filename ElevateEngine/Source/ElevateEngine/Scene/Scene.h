#pragma once

#include <memory>
#include <entt/entt.hpp>

#include <ElevateEngine/Core/Enums.h>
#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/EEObjectPtr.h>

#include <ElevateEngine/Renderer/Cubemap.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

// Forward declarations
namespace Elevate
{
	class Cubemap;
	class Shader;
	class GameObject;
	class Camera;
	class ComponentRegistry;
	class RendererAPI;
	class Event;
}

namespace Elevate
{
	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;

	enum class SceneType : EnumType
	{
		RuntimeScene = 0,
		EditorScene = 1,
		DebugScene = 99
	};

	BEGIN_ENUM(SceneType)
		ENUM_VALUE(RuntimeScene)
		ENUM_VALUE(RuntimeScene)
		ENUM_VALUE(RuntimeScene)
	END_ENUM(SceneType)

	class Scene : public EEObject
	{
		BEGIN_OBJECT(Scene)
		using Super = Scene;

	public:
		Scene();
		Scene(std::string name, SceneType type = SceneType::RuntimeScene);
		//~Scene(); // todo check if we remove in the future
		~Scene() = default;

		void OnAwake();
		void OnStart();
		void UpdateScene();
		void RenderScene(Camera* cam = nullptr);
		void Notify(Event& event); // Dispatch an event to gameobjects
		virtual std::string GetName() const override;

		void AddObject(const EEObjectPtr<GameObject>& newObject, const EEObjectPtr<GameObject>& parent);
		const std::set<EEObjectPtr<GameObject>> GetRootObjects() const { return m_rootObjects; }

		inline SceneType GetType() { return m_type; }

		static ScenePtr Create(std::string name, SceneType type = SceneType::RuntimeScene);
			
		// Cubemap
		void SetSkybox(const std::string& skyboxFilePath);
		std::weak_ptr<Cubemap> GetSkybox();

		SceneLighting* GetSceneLighting();
	private:
		void RemoveFromRoot(const EEObjectPtr<GameObject>& object);
		void AddRootObject(const EEObjectPtr<GameObject>& newRootObject);

	private:
		static uint32_t s_nextRegistryId;

	private:
		std::string m_name;
		PROPERTY(m_name);

		SceneType m_type;
		PROPERTY(m_type)

		// Component registry id for all entt::entity
		uint32_t m_registryId;

		// Root objects of the scene hierarchy.
		std::set<EEObjectPtr<GameObject>> m_rootObjects;

		std::shared_ptr<Cubemap> m_cubemap;
		std::unique_ptr<SceneLighting> m_sceneLighting = nullptr;

		END_OBJECT_CUSTOM()
		DECLARE_AUTO_OBJECT_LAYOUT()
		
		friend class GameObject;
		friend class ComponentRegistry;
	};
}