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

void Elevate::GameObject::OnSetPosition()
{
	// TODO VERIF SI VRARIMENT UTILE
}

Elevate::GameObject::~GameObject()
{
	if (m_Scene)
	{
		m_Scene->m_Registry.destroy(m_Entity);
	} else EE_CORE_ERROR("Object '{0}' must be destroyed from an existing scene!", m_Name);
}

void Elevate::GameObject::SetParent(GameObjectPtr newParent)
{
	if (newParent)
	{
		this->m_Parent = newParent;
		newParent->SetChild(shared_from_this());
		m_Scene->RemoveFromRoot(shared_from_this());
	}
}

void Elevate::GameObject::SetChild(GameObjectPtr child)
{
	if (child)
	{
		child->m_Parent = shared_from_this();
		m_Childs.emplace(child);
		m_Scene->RemoveFromRoot(child);
	}
}

void Elevate::GameObject::RemoveChild(GameObjectPtr child)
{
	// TODO RemoveChild();
}

// TODO MOVE SOMEWHERE ELSE
glm::mat4 Elevate::GameObject::GenGlobalMatrix() const
{
	if (m_Parent)
	{
		return m_Parent->GenGlobalMatrix() * GetModelMatrix();
	} 
	else
	{
		return GetModelMatrix();
	}
}

Elevate::GameObjectPtr Elevate::GameObject::Create(std::string name, ScenePtr scene, GameObjectPtr parent)
{
	GameObjectPtr obj = std::make_shared<GameObject>(name, scene, parent);
	obj->Initialize();
	obj->SetPosition({ 0.0f, 0.0f, 0.0f });
	return obj;
}
