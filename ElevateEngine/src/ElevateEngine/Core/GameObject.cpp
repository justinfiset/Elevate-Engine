#include "eepch.h"
#include "GameObject.h"

#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Scene/Scene.h>

#include <imgui.h>
#include "ImGuizmo.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Core/ComponentRegistry.h>
#include <ElevateEngine/Core/Log.h>


Elevate::GameObject::GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
	: m_name(name), m_scene(scene.get()), m_parent(parent) { }

void Elevate::GameObject::SetFromGlobalMatrix(const glm::mat4& newWorld)
{
	glm::mat4 newLocal;
	if (m_parent)
	{
		newLocal = glm::inverse(m_parent->GenGlobalMatrix()) * newWorld;
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

	for (std::shared_ptr<Elevate::GameObject> child : m_childs)
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

	for (std::shared_ptr<Elevate::GameObject> child : m_childs)
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

	for (std::shared_ptr<Elevate::GameObject> child : m_childs)
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

	for (std::shared_ptr<Elevate::GameObject> child : m_childs)
	{
		child->Notify(e);
	}
}

void Elevate::GameObject::Initialize()
{
	if (m_scene)
	{
		if (m_parent)
		{
			m_parent->AddChild(shared_from_this());
		}
		else
		{
			m_scene->AddRootObject(shared_from_this());
		}

		entt::entity entity = m_scene->m_Registry.create();
		m_entityId = static_cast<std::uint32_t>(entity);
	}
	else
	{
		EE_CORE_ERROR("Object '{0}' must be linked with an existing scene!", m_name);
	}
}

void Elevate::GameObject::OnSetPosition()
{

}

Elevate::GameObject::~GameObject()
{
	if (m_scene)
	{
		m_scene->m_Registry.destroy(entt::entity(m_entityId));
	} else EE_CORE_ERROR("Object '{0}' must be destroyed from an existing scene!", m_name);
}

namespace Elevate
{
	std::vector<Component*> Elevate::GameObject::GetComponents() const
	{
		std::vector<Component*> components;

		if (!m_scene) return components;

		entt::registry& registry = m_scene->m_Registry;
		const entt::entity entity = entt::entity(m_entityId);

		for (auto& [type, entry] : ComponentRegistry::GetEntries()) {
			if (Component* component = entry.creator(registry, entity)) {
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

		for (std::shared_ptr<Elevate::GameObject> child : m_childs)
		{
			child->RenderInEditor();
		}
	}
}

void Elevate::GameObject::SetParent(std::shared_ptr<GameObject> newParent)
{
	if (newParent == m_parent)
		return;
	
	if (m_parent) 
	{
		m_parent->RemoveChild(shared_from_this());
	}

	this->m_parent = newParent;

	if (newParent)
	{
		newParent->AddChild(shared_from_this());
		if (m_scene)
		{
			m_scene->RemoveFromRoot(shared_from_this());
		}
	} else {
		m_scene->AddRootObject(shared_from_this());
	}
}

void Elevate::GameObject::Destroy()
{
	if (m_parent) {
		m_parent->RemoveChild(shared_from_this());
	}
	else {
		m_scene->RemoveFromRoot(shared_from_this());
	}

	auto childsCopy = m_childs;
	for (const auto& child : childsCopy)
	{
		if (child)
		{
			child->Destroy();
		}
	}
	m_childs.clear();
	m_parent.reset();
}

void Elevate::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (child)
	{
		child->m_parent = shared_from_this();
		m_childs.emplace(child);
		m_scene->RemoveFromRoot(child);
	}
}

void Elevate::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	m_childs.erase(child);
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
	if (m_parent)
	{
		return m_parent->GenGlobalMatrix() * GetModelMatrix();
	} 
	else
	{
		return GetModelMatrix();
	}
}
