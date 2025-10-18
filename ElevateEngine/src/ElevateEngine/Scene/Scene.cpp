#include "eepch.h"
#include "Scene.h"

#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/Application.h>

#include <ElevateEngine/Scene/SceneManager.h>

#include <ElevateEngine/Renderer/Model.h>
#include <ElevateEngine/Renderer/Renderer.h>

#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Camera/CameraManager.h>

#include "ScenePrivate.h"

// rapidjson
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace Elevate
{
	std::uint32_t Scene::s_nextRegistryId = 1;

	Scene::Scene() : Scene("DefaultScene", SceneType::RuntimeScene) { }

	Scene::Scene(std::string name, SceneType type) : m_name(name), m_type(type)
	{
		m_registryId = s_nextRegistryId;
		s_nextRegistryId++;
		GetRegistryMap()[m_registryId] = std::make_unique<entt::registry>();

		EE_TRACE("Created scene '%s' with registry id: %d", m_name.c_str(), m_registryId);
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
		if (m_type == SceneType::RuntimeScene && Application::GetGameState() != Runtime)
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
		if (!cam)
		{
			cam = Elevate::CameraManager::GetCurrent();
		}

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
			switch (m_type)
			{
			case EditorScene:
				if (Application::GetGameState() == EditorMode)
				{
					obj->Render();
				}

				break;
			case RuntimeScene:
				obj->Render();

				if (Application::GetGameState() == EditorMode)
				{
					obj->RenderInEditor();
				}

				break;
			case DebugScene:
				obj->Render();
				break;
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
		scene->Serialize();
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

	std::string Scene::Serialize() const
	{
		rapidjson::Document doc;
		doc.SetObject();
		auto& allocator = doc.GetAllocator();

		doc.AddMember("name", rapidjson::Value(m_name.c_str(), allocator), allocator);
		doc.AddMember("type", rapidjson::Value().SetInt(m_type), allocator);

		// todo for objetcs
		// todo for cubemap
		// todo for SceneLighting

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		EE_CORE_TRACE("{}", buffer.GetString());
		return buffer.GetString();
	}
}