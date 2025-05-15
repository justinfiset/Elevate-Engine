#include "eepch.h"
#include "Scene.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/ComponentWrapper.h"

#include "ElevateEngine/Renderer/Model.h"

void Elevate::Scene::UpdateScene()
{
	m_Registry.view<ComponentWrapper>().each([](auto& wrapper) 
	{
		if (wrapper.IsActive())
		{
			wrapper.Update();
		}
	});
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

	m_Registry.view<ComponentWrapper>().each([](auto& wrapper)
	{
		if (wrapper.IsActive())
		{
			wrapper.Render();
		}
	});
}

void Elevate::Scene::Notify(Event& e)
{
	m_Registry.view<ComponentWrapper>().each([&e](auto& wrapper)
	{
		if (wrapper.IsActive())
		{
			wrapper.OnNotify(e);
		}
	});
}

void Elevate::Scene::AddObject(GameObjectPtr newObject, GameObjectPtr parent)
{
	// TODO TEST IN DEPTH
	if (!parent)
	{
		AddRootObject(newObject);
	}
	else
	{
		parent->SetChild(newObject);
		newObject->SetParent(parent);
	}
}

Elevate::ScenePtr Elevate::Scene::Create(std::string name, SceneType type)
{
	return std::make_shared<Scene>(name, type);
}

void Elevate::Scene::SetSkybox(Cubemap* cubemap)
{
	m_cubemap.reset(cubemap);
}

std::weak_ptr<Elevate::Cubemap> Elevate::Scene::GetSkybox()
{
	return m_cubemap;
}

void Elevate::Scene::RemoveFromRoot(GameObjectPtr object)
{
	m_rootObjects.erase(object);
}

void Elevate::Scene::AddRootObject(GameObjectPtr newRootObject)
{
	// TODO MAYBE REMOVE IN THE FUTURE IS THE APP IS FOOL PROOF
	newRootObject->m_Parent = nullptr;
	newRootObject->m_Scene = this;
	m_rootObjects.insert(newRootObject);
}