module;

#include <format>
#include <type_traits>

module Elevate.Core.Scene;

// Foundations
import Elevate.Foundations.Assert;
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Application;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Renderer.Cubemap;
import Elevate.Renderer.SceneLighting;

namespace Elevate
{
	std::uint32_t Scene::s_nextRegistryId = 1;

	Scene::Scene() : Scene(nullptr, "DefaultScene", SceneType::RuntimeScene) {}

	Scene::Scene(SceneManager* sceneManager, std::string name, SceneType type) : m_name(name), m_type(type)
	{
		Assert::That(sceneManager != nullptr, "Scene Manager is null.");
		m_sceneManager = sceneManager;

		m_registryId = s_nextRegistryId;
		s_nextRegistryId++;
		m_sceneManager->GetComponentRegistry().CreateRegistry(m_registryId);

		m_sceneLighting = std::make_unique<SceneLighting>();

		CoreLogger::Trace("Created scene '{}' with registry id: {}", m_name.c_str(), m_registryId);
	}

	Scene::~Scene()
	{
	}

	void Scene::OnAwake()
	{
		for (const auto& obj : m_rootObjects)
		{
			obj->Awake();
		}
	}

	void Scene::OnStart()
	{
		for (const auto& obj : m_rootObjects)
		{
			obj->Start();
		}
	}

	std::string Scene::GetName() const
	{
		return m_name;
	}

	void Scene::UpdateScene()
	{
		if (m_type == SceneType::RuntimeScene && Application::GetGameState() != Runtime)
		{
			return;
		}

		for (const auto& obj : m_rootObjects)
		{
			obj->Update();
		}
	}

	void Scene::RenderScene()
	{
		for (EEObjectPtr<GameObject> obj : m_rootObjects)
		{
			switch (m_type)
			{
			case SceneType::EditorScene:
				if (Application::GetGameState() == EditorMode)
				{
					obj->Render();
				}

				break;
			case SceneType::RuntimeScene:
				obj->Render();

				if (Application::GetGameState() == EditorMode)
				{
					obj->RenderInEditor();
				}

				break;
			case SceneType::DebugScene:
				obj->Render();
				break;
			}
		}
	}

	void Scene::Notify(Event& e)
	{
		for (EEObjectPtr<GameObject> obj : m_rootObjects)
		{
			obj->Notify(e);
		}
	}

	void Scene::AddObject(const EEObjectPtr<GameObject>& newObject, const EEObjectPtr<GameObject>& parent)
	{
		if (!newObject)
			return;

		if (!parent)
		{
			AddRootObject(newObject);
		}
		else
		{
			if (newObject->GetScene() == parent->GetScene())
			{
				newObject->SetParent(parent);
			}
			else
			{
				CoreLogger::Error("Cannot add a child object from a different scene.");
			}
		}
	}

	ComponentRegistry& Scene::GetComponentRegistry()
	{
		Assert::That(m_sceneManager != nullptr, "SceneManager is null.");
		return m_sceneManager->GetComponentRegistry();
	}

	void Scene::SetSkybox(const std::string& skyboxFilePath)
	{
		m_cubemap.reset(Cubemap::CreateFromFile(skyboxFilePath));
	}

	std::weak_ptr<Cubemap> Scene::GetSkybox()
	{
		return m_cubemap;
	}

	SceneLighting* Scene::GetSceneLighting()
	{
		return m_sceneLighting.get();
	}

	uint32_t Scene::GetRegistryId()
	{
		return m_registryId;
	}

	void Scene::RemoveFromRoot(const EEObjectPtr<GameObject>& object)
	{
		auto it = std::find(m_rootObjects.begin(), m_rootObjects.end(), object);
		if (it != m_rootObjects.end())
		{
			m_rootObjects.erase(it);
		}
	}

	void Scene::AddRootObject(const EEObjectPtr<GameObject>& newRootObject)
	{
		newRootObject->m_parent.reset();
		newRootObject->m_scene = this;
		m_rootObjects.push_back(newRootObject);
	}
}