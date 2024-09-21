#include "eepch.h"
#include "GameObject.h"
#include "ElevateEngine/Scene/Scene.h"

Elevate::GameObject::GameObject(std::string name) : m_Name(name)
{
}

Elevate::GameObject::GameObject(std::string name, Scene* scene) : m_Name(name), m_Scene(scene)
{
	if (m_Scene != NULL)
	{
		m_Entity = m_Scene->m_Registry.create();
	}
	else EE_CORE_ERROR("Object '{0}' must be linked with an existing scene!", m_Name);
}

Elevate::GameObject::~GameObject()
{
	// TODO trouver comment implementer comme du monde
	//if (m_Scene != NULL)
	//{
	//	m_Scene->m_Registry.destroy(m_Entity);
	//} else EE_CORE_ERROR("Object '{0}' must be destroyed from an existing scene!", m_Name);
}
