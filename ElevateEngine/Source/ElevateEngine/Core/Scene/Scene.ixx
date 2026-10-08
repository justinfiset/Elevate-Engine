module;

#include <memory>
#include <vector>

#include <ElevateEngine/Core/Reflection/Reflection.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

export module Elevate.Core.Scene.Scene;

import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene.ComponentRegistry;
import Elevate.Core.Scene.SceneType;

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

export namespace Elevate
{
	class Scene : public EEObject
	{
		BEGIN_OBJECT(Scene);
		using Super = EEObject;

	public:
		Scene();
		Scene(std::string name, SceneType type = SceneType::RuntimeScene);
		virtual ~Scene();

		void OnAwake();
		void OnStart();
		void UpdateScene();
		void RenderScene(Camera* cam = nullptr);
		void Notify(Event& event); // Dispatch an event to gameobjects
		virtual std::string GetName() const override;

		void AddObject(const EEObjectPtr<GameObject>& newObject, const EEObjectPtr<GameObject>& parent);
		const std::vector<EEObjectPtr<GameObject>>& GetRootObjects() const { return m_rootObjects; }

		inline SceneType GetType() { return m_type; }

		static std::shared_ptr<Scene> Create(std::string name, SceneType type = SceneType::RuntimeScene);

		// Cubemap -> todo : move in a more specific struct
		void SetSkybox(const std::string& skyboxFilePath);
		std::weak_ptr<Cubemap> GetSkybox();

		SceneLighting* GetSceneLighting();

		uint32_t GetRegistryId();

		void RemoveFromRoot(const EEObjectPtr<GameObject>& object);
		void AddRootObject(const EEObjectPtr<GameObject>& newRootObject);

	private:
		static uint32_t s_nextRegistryId;

		std::string m_name;
		PROPERTY(m_name);

		SceneType m_type;
		PROPERTY(m_type)

			// Component registry id for all entt::entity
			uint32_t m_registryId;

		// Root objects of the scene hierarchy.
		std::vector<EEObjectPtr<GameObject>> m_rootObjects;
		PROPERTY(m_rootObjects)

			std::shared_ptr<Cubemap> m_cubemap;
		std::unique_ptr<SceneLighting> m_sceneLighting = nullptr;

		END_OBJECT_CUSTOM();
		DECLARE_AUTO_OBJECT_LAYOUT();
	};
}