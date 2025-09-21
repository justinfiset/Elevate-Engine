#include "eepch.h"
#include "GameObject.h"
#include "ElevateEngine/Scene/Scene.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Core/ComponentRegistry.h>

Elevate::GameObject::GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
	: m_Name(name), m_Scene(scene.get()), m_Parent(parent) { }

void Elevate::GameObject::SetFromGlobalMatrix(const glm::mat4& newWorld)
{
	glm::mat4 newLocal;
	if (m_Parent)
	{
		newLocal = glm::inverse(m_Parent->GenGlobalMatrix()) * newWorld;
	}
	else
	{
		newLocal = newWorld;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	ImGuizmo::DecomposeMatrixToComponents
	(
		glm::value_ptr(newLocal),
		glm::value_ptr(position),
		glm::value_ptr(rotation),
		glm::value_ptr(scale)
	);

	// TODO SHOULD SET GLOBAL INSTEAD OF LOCAL, THEREFORE THESE DO NOT ALWYAS WORKS ON CHILDS FOR THE MOMENT
	// -> IL FAUDRAIT UNE MÉTHODE QUI UPDATE LE TOUT À PARTIR D'UNE MÉTHODE SetFromGlobalMatrix() qui set le tout
	SetScale(scale);
	SetRotation(rotation);
	SetPosition(position);
}

glm::vec3 Elevate::GameObject::GetGlobalPosition()
{
	glm::mat4 global = GenGlobalMatrix();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	ImGuizmo::DecomposeMatrixToComponents
	(
		glm::value_ptr(global),
		glm::value_ptr(position),
		glm::value_ptr(rotation),
		glm::value_ptr(scale)
	);

	return position;
}

void Elevate::GameObject::Update()
{
	for (Component* comp : GetComponents())
	{
		if (comp->IsActive())
		{
			comp->Update();
		}
	}

	for (std::shared_ptr<Elevate::GameObject> child : m_Childs)
	{
		child->Update();
	}
}

void Elevate::GameObject::PreRender()
{
	for (Component* comp : GetComponents())
	{
		if (comp->IsActive())
		{
			comp->PreRender();
		}
	}

	for (std::shared_ptr<Elevate::GameObject> child : m_Childs)
	{
		child->PreRender();
	}
}

void Elevate::GameObject::Render()
{
	for (Component* comp : GetComponents())
	{
		if (comp->IsActive())
		{
			comp->Render();
		}
	}

	for (std::shared_ptr<Elevate::GameObject> child : m_Childs)
	{
		child->Render();
	}
}

void Elevate::GameObject::Notify(Event& e)
{
	for (Component* comp : GetComponents())
	{
		if (comp->IsActive())
		{
			comp->OnNotify(e);
		}
	}

	for (std::shared_ptr<Elevate::GameObject> child : m_Childs)
	{
		child->Notify(e);
	}
}

void Elevate::GameObject::Initialize()
{
	if (m_Scene)
	{
		if (m_Parent)
		{
			m_Parent->AddChild(shared_from_this());
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

}

Elevate::GameObject::~GameObject()
{
	if (m_Scene)
	{
		m_Scene->m_Registry.destroy(m_Entity);
	} else EE_CORE_ERROR("Object '{0}' must be destroyed from an existing scene!", m_Name);
}

namespace Elevate
{
	std::vector<Component*> Elevate::GameObject::GetComponents() const
	{
		std::vector<Component*> components;

		if (!m_Scene) return components;

		entt::registry& registry = m_Scene->m_Registry;
		const entt::entity entity = m_Entity;

		for (const auto& [type, factory] : ComponentRegistry::GetFactories()) {
			if (Component* component = factory(registry, entity)) {
				components.push_back(component);
			}
		}

		return components;
	}

	void Elevate::GameObject::RenderInEditor()
	{
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->RenderInEditor();
			}
		}

		for (std::shared_ptr<Elevate::GameObject> child : m_Childs)
		{
			child->RenderInEditor();
		}
	}
}

void Elevate::GameObject::SetParent(std::shared_ptr<GameObject> newParent)
{
	if (newParent == m_Parent)
		return;
	
	if (m_Parent) 
	{
		m_Parent->RemoveChild(shared_from_this());
	}

	this->m_Parent = newParent;

	if (newParent)
	{
		newParent->AddChild(shared_from_this());
		if (m_Scene)
		{
			m_Scene->RemoveFromRoot(shared_from_this());
		}
	} else {
		m_Scene->AddRootObject(shared_from_this());
	}
}

void Elevate::GameObject::Destroy()
{
	if (m_Parent) {
		m_Parent->RemoveChild(shared_from_this());
	}
	else {
		m_Scene->RemoveFromRoot(shared_from_this());
	}

	auto childsCopy = m_Childs;
	for (const auto& child : childsCopy)
	{
		if (child)
		{
			child->Destroy();
		}
	}
	m_Childs.clear();
	m_Parent.reset();
}

void Elevate::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (child)
	{
		child->m_Parent = shared_from_this();
		m_Childs.emplace(child);
		m_Scene->RemoveFromRoot(child);
	}
}

void Elevate::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	m_Childs.erase(child);
}

std::shared_ptr<Elevate::GameObject> Elevate::GameObject::Create(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
{
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(name, scene, parent);
	obj->Initialize();
	obj->SetPosition({ 0.0f, 0.0f, 0.0f });
	return obj;
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
