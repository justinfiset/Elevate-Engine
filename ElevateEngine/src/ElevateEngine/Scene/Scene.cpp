#include "eepch.h"
#include "Scene.h"

#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/GameContext.h>
#include <ElevateEngine/Core/Application.h>

#include <ElevateEngine/Scene/SceneManager.h>

#include <ElevateEngine/Renderer/Model.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Camera.h>

#include "ScenePrivate.h"

namespace Elevate
{
	std::uint32_t Scene::s_nextRegistryId = 1;

	Scene::Scene() : Scene("DefaultScene", SceneType::RuntimeScene) { }

	Scene::Scene(std::string name, SceneType type) : m_name(name), m_type(type)
	{
		m_registryId = s_nextRegistryId;
		s_nextRegistryId++;
		GetRegistryMap()[m_registryId] = std::make_unique<entt::registry>();

		EE_TRACE("Created scene '{}' with registry id: {}", m_name, m_registryId);
	}

	//Scene::~Scene()
	//{
	//	auto& registryMap = GetRegistryMap();
	//	auto registryIt = registryMap.find(m_registryId);

	//	if (registryIt != registryMap.end()) {
	//		registryMap.erase(registryIt);
	//	}
	//}

	void Scene::UpdateScene()
	{
		if (m_type == SceneType::RuntimeScene && Application::GameState() != Runtime)
		{
			return;
		}

		for (std::shared_ptr<GameObject> obj : m_rootObjects)
		{
			obj->Update();
		}
	}

	void Scene::RenderScene(Camera* cam)
	{
		// Render the cubemap / skybox
		if (cam)
		{
			// Either do not calculate here or stop calculating it in the layer
			glm::mat4 view = glm::mat4(glm::mat3(cam->GenViewMatrix()));

			if (m_cubemap)
			{
				m_cubemap->SetProjectionMatrix(cam->GetProjectionMatrix());
				m_cubemap->SetViewMatrix(view);
				m_cubemap->Draw();
			}
		}

		for (std::shared_ptr<GameObject> obj : m_rootObjects)
		{
			obj->PreRender();
		}

		if (m_sceneLighting)
		{
			Renderer::SetupShaders(this);
		}

		for (std::shared_ptr<GameObject> obj : m_rootObjects)
		{
			obj->Render();

			// Call specific functions depending on current context status.
			switch (Application::GameState())
			{
			case GameContextState::EditorMode:
				obj->RenderInEditor();
			}
		}
	}

	void Scene::Notify(Event& e)
	{
		for (std::shared_ptr<GameObject> obj : m_rootObjects)
		{
			obj->Notify(e);
		}
	}

	void Scene::AddObject(std::shared_ptr<GameObject> newObject, std::shared_ptr<GameObject> parent)
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
				EE_CORE_ERROR("Cannot add a child object from a different scene.");
			}
		}
	}

	ScenePtr Scene::Create(std::string name, SceneType type)
	{
		ScenePtr scene = std::make_shared<Scene>(name, type);
		SceneManager::LoadScene(scene);
		return scene;
	}

	void Scene::SetSkybox(const char* skyboxFilePath)
	{
		m_cubemap.reset(Cubemap::CreateFromFile(skyboxFilePath));
	}

	std::weak_ptr<Cubemap> Scene::GetSkybox()
	{
		return m_cubemap;
	}

	void Scene::RemoveFromRoot(std::shared_ptr<GameObject> object)
	{
		m_rootObjects.erase(object);
	}

	void Scene::AddRootObject(std::shared_ptr<GameObject> newRootObject)
	{
		newRootObject->m_parent = nullptr;
		newRootObject->m_scene = this;
		m_rootObjects.insert(newRootObject);
	}
}