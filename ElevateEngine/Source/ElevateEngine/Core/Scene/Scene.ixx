module;

#include <memory>
#include <vector>

#include <ElevateEngine/Core/Reflection/Reflection.h>
export module Elevate.Core.Scene:Scene;

import Elevate.Core.Events.Event;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Renderer.Cubemap;
import Elevate.Renderer.SceneLighting;

import :Fwd;
import :ComponentRegistry;
import :SceneType;

// Forward declarations
namespace Elevate
{
	class RendererAPI;
}

export namespace Elevate
{
	class Scene : public EEObject
	{
		BEGIN_OBJECT(Scene);
		using Super = EEObject;

	public:
		Scene();
		Scene(SceneManager* sceneManager, std::string name, SceneType type = SceneType::RuntimeScene);
		virtual ~Scene();

		void OnAwake();
		void OnStart();
		void UpdateScene();
		void RenderScene();
		void Notify(Event& event); // Dispatch an event to gameobjects
		virtual std::string GetName() const override;

		void AddObject(const EEObjectPtr<GameObject>& newObject, const EEObjectPtr<GameObject>& parent);
		const std::vector<EEObjectPtr<GameObject>>& GetRootObjects() const { return m_rootObjects; }

		inline SceneType GetType() { return m_type; }

		// Cubemap -> todo : move in a more specific struct
		void SetSkybox(const std::string& skyboxFilePath);
		std::weak_ptr<Cubemap> GetSkybox();

		SceneLighting* GetSceneLighting();

		uint32_t GetRegistryId();

		ComponentRegistry& GetComponentRegistry();

		void RemoveFromRoot(const EEObjectPtr<GameObject>& object);
		void AddRootObject(const EEObjectPtr<GameObject>& newRootObject);

	private:
		static uint32_t s_nextRegistryId;
		SceneManager* m_sceneManager = nullptr;

		std::string m_name;
		PROPERTY(m_name);

		SceneType m_type;
		PROPERTY(m_type);

		// Component registry id for all entt::entity
		uint32_t m_registryId;

		// Root objects of the scene hierarchy.
		std::vector<EEObjectPtr<GameObject>> m_rootObjects;
		PROPERTY(m_rootObjects);

		std::shared_ptr<Cubemap> m_cubemap;
		std::unique_ptr<SceneLighting> m_sceneLighting = nullptr;

		END_OBJECT_CUSTOM();
		DECLARE_AUTO_OBJECT_LAYOUT();
	};
}