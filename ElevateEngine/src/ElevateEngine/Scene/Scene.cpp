#include "eepch.h"
#include "Scene.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/ComponentWrapper.h"

#include "ElevateEngine/Renderer/Model.h"

Elevate::Scene::Scene()
{
	
}

Elevate::Scene::~Scene()
{
}

void Elevate::Scene::UpdateScene()
{
	m_Registry.view<ComponentWrapper>().each([](auto& wrapper) 
	{
		if (wrapper.component->IsActive())
		{
			wrapper.component->Update();
		}
	});
}

void Elevate::Scene::RenderScene()
{
	m_Registry.view<Model>().each([](auto& wrapper) 
	{
		if (wrapper.IsActive())
		{
			wrapper.Render();
		}
	});
}

void Elevate::Scene::AddRootObject(std::shared_ptr<GameObject> newRootObject)
{
	newRootObject->m_Scene = this;
	m_RootObjects.insert(newRootObject);
}