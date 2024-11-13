#include "eepch.h"
#include "GameObject.h"
#include "ElevateEngine/Scene/Scene.h"

Elevate::GameObject::GameObject(std::string name, ScenePtr scene, GameObjectPtr parent) 
	: m_Name(name), m_Scene(scene.get()), m_Parent(parent) { }

void Elevate::GameObject::Initialize()
{
	if (m_Scene)
	{
		if (m_Parent)
		{
			m_Parent->SetChild(shared_from_this());
		}
		else
		{
			m_Scene->AddRootObject(shared_from_this());
		}

		m_Entity = m_Scene->m_Registry.create();
	}
	else
	{
		EE_CORE_ERROR("Object '{0}' must be linked with an existing scene!", m_Name);
	}
}

Elevate::GameObject::~GameObject()
{
	// TODO trouver comment implementer comme du monde
	//if (m_Scene)
	//{
	//	m_Scene->m_Registry.destroy(m_Entity);
	//} else EE_CORE_ERROR("Object '{0}' must be destroyed from an existing scene!", m_Name);
}

void Elevate::GameObject::SetChild(GameObjectPtr child)
{
	child->m_Parent = shared_from_this();
	m_Childs.emplace(child);
}

void Elevate::GameObject::RemoveChild(GameObjectPtr child)
{
	// TODO RemoveChild();
}

Elevate::GameObjectPtr Elevate::GameObject::Create(std::string name, ScenePtr scene, GameObjectPtr parent)
{
	GameObjectPtr obj = std::make_shared<GameObject>(name, scene, parent);
	obj->Initialize();
	return obj;
}
