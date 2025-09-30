#include "eepch.h"
#include "Scene.h"

#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/GameContext.h>
#include <ElevateEngine/Core/Application.h>

#include <ElevateEngine/Scene/SceneManager.h>

#include <ElevateEngine/Renderer/Model.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Camera.h>

void Elevate::Scene::UpdateScene()
{
	if (m_Type == SceneType::RuntimeScene && Application::GameState() != Runtime)
	{
		return;
	}

	for (std::shared_ptr<GameObject> obj : m_rootObjects)
	{
		obj->Update();
	}
}

void Elevate::Scene::RenderScene(Camera* cam)
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

void Elevate::Scene::Notify(Event& e)
{
	for (std::shared_ptr<GameObject> obj : m_rootObjects)
	{
		obj->Notify(e);
	}
}

void Elevate::Scene::AddObject(std::shared_ptr<GameObject> newObject, std::shared_ptr<GameObject> parent)
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

Elevate::ScenePtr Elevate::Scene::Create(std::string name, SceneType type)
{
	ScenePtr scene = std::make_shared<Scene>(name, type);
	SceneManager::LoadScene(scene);
	return scene;
}

void Elevate::Scene::SetSkybox(const char* skyboxFilePath)
{
	m_cubemap.reset(Cubemap::CreateFromFile(skyboxFilePath));
}

std::weak_ptr<Elevate::Cubemap> Elevate::Scene::GetSkybox()
{
	return m_cubemap;
}

void Elevate::Scene::RemoveFromRoot(std::shared_ptr<GameObject> object)
{
	m_rootObjects.erase(object);
}

void Elevate::Scene::AddRootObject(std::shared_ptr<GameObject> newRootObject)
{
	newRootObject->m_parent = nullptr;
	newRootObject->m_scene = this;
	m_rootObjects.insert(newRootObject);
}