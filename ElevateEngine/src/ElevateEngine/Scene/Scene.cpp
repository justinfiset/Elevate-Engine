#include "eepch.h"
#include "Scene.h"
#include "ElevateEngine/Core/GameObject.h"

Elevate::Scene::Scene()
{
	
}

Elevate::Scene::~Scene()
{
}

void Elevate::Scene::AddRootObject(std::shared_ptr<GameObject> newRootObject)
{
	newRootObject->m_Scene = this;
	m_RootObjects.insert(newRootObject);
}