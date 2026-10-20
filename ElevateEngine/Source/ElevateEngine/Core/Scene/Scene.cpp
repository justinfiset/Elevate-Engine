module;

// rapidjson
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <ElevateEngine/Renderer/Models/Model.h>
#include <ElevateEngine/Renderer/Renderer/Renderer.h>

#include <ElevateEngine/Renderer/Light/SceneLighting.h>

export module Elevate.Core.Scene:Scene;

import Elevate.Core.Application;
import Elevate.Core.Objects.ObjectPtr;

import Elevate.Core.Scene:GameObject;
import Elevate.Core.Scene.SceneManager;

import Elevate.Renderer.Camera.Camera;
import Elevate.Renderer.Camera.CameraManager;

namespace Elevate
{
	std::uint32_t Scene::s_nextRegistryId = 1;

	Scene::Scene() : Scene("DefaultScene", SceneType::RuntimeScene) {}

	Scene::Scene(std::string name, SceneType type) : m_name(name), m_type(type)
	{
		m_registryId = s_nextRegistryId;
		s_nextRegistryId++;
		SceneManager::GetComponentRegistry()->CreateRegistry(m_registryId);

		m_sceneLighting = std::make_unique<SceneLighting>();

		CoreLogger::Trace("Created scene '{}' with registry id: {}", m_name.c_str(), m_registryId);
	}

	Scene::~Scene()
	{
		Application::Get()->Registry
			DestroyRegistry(m_registryId);
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

	void Scene::RenderScene(Camera* cam)
	{
		if (!cam)
		{
			cam = Elevate::CameraManager::GetCurrent();
		}

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

	std::shared_ptr<Scene> Scene::Create(std::string name, SceneType type)
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>(name, type);
		SceneManager::LoadScene(scene);
		return scene;
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