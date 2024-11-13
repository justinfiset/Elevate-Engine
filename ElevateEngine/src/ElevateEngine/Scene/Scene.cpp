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
		if (wrapper.IsActive())
		{
			wrapper.Update();
		}
	});
}

void Elevate::Scene::RenderScene()
{
	// TODO REMETTRE AVEC LES WRAPPERS
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
	// TODO IMPL
}

void Elevate::Scene::AddRootObject(GameObjectPtr newRootObject)
{
	// TODO MAYBE REMOVE IN THE FUTURE IS THE APP IS FOOL PROOF
	newRootObject->m_Parent = nullptr;
	newRootObject->m_Scene = this;
	m_RootObjects.insert(newRootObject);
}