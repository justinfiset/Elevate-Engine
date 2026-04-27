#include "GameObject.h"

#include <format>

#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <ElevateEngine/Audio/SoundEngine.h>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/ComponentRegistry.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Scene/Scene.h>
#include <ElevateEngine/Scene/ScenePrivate.h>


namespace Elevate
{
	uint32_t GameObject::s_goIdCount = 0;

	GameObject::GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
		: m_name(name), m_scene(scene.get()), m_parent(parent)
	{
		m_goId = s_goIdCount;
		s_goIdCount++;
		EE_CORE_TRACE("Created object '{}', id='{}', GUID='{}'", m_name, m_goId, m_guid.ToString());
	}

	void GameObject::SetFromGlobalMatrix(const glm::mat4& newWorld)
	{
		glm::mat4 newLocal = m_parent ? glm::inverse(m_parent->GenGlobalMatrix()) * newWorld : newWorld;

		glm::vec3 scale;
		glm::quat rotationQuat;
		glm::vec3 position;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(newLocal, scale, rotationQuat, position, skew, perspective);
		glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(rotationQuat));

		SetScale(scale);
		SetRotation(rotationEuler);
		SetPosition(position);
	}

	glm::vec3 GameObject::GetGlobalPosition()
	{
		glm::mat4 global = GenGlobalMatrix();
		return glm::vec3(global[3]);
	}

	void GameObject::Update()
	{
		// TODO MAKE GETCOMPONENTS ONLY RETURN ACTIVE COMOPNENTS TO PREVENT THE CHECK
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->Update();
			}
		}

		for (const auto& child : m_childs)
		{
			child->Update();
		}
	}

	void GameObject::Render()
	{
		// TODO MAKE GETCOMPONENTS ONLY RETURN ACTIVE COMOPNENTS TO PREVENT THE CHECK
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->Render();
			}
		}

		for (const auto& child : m_childs)
		{
			child->Render();
		}
	}

	void GameObject::Notify(Event& e)
	{
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->OnNotify(e);
			}
		}

		for (const auto& child : m_childs)
		{
			child->Notify(e);
		}
	}

	void GameObject::OnSetPosition()
	{
		if (m_entityId != EE_INVALID_ENTITY_ID && m_isInitialized)
		{
			SoundEngine::UpdatePosition(this);
		}

		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->OnSetPosition();
			}
		}

		for (const auto& child : m_childs)
		{
			child->OnSetPosition();
		}
	}

	void GameObject::OnSetRotation()
	{
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->OnSetRotation();
			}
		}

		for (const auto& child : m_childs)
		{
			child->OnSetRotation();
		}
	}

	void GameObject::OnSetScale()
	{
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->OnSetScale();
			}
		}

		for (const auto& child : m_childs)
		{
			child->OnSetScale();
		}
	}

	void GameObject::Initialize()
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

			auto& registryMap = GetRegistryMap();
			auto registryIt = registryMap.find(m_scene->m_registryId);

			if (registryIt == registryMap.end()) {
				EE_CORE_ERROR("Registry not found for scene '%s' with id: %d",
					m_scene->GetName(), m_scene->m_registryId);
				return;
			}
			if (!registryIt->second) {
				EE_CORE_ERROR("Registry pointer is null for scene '%s' with id: %d",
					m_scene->GetName(), m_scene->m_registryId);
				return;
			}
			entt::entity entity = registryIt->second->create();
			m_entityId = static_cast<std::uint32_t>(entity);

			SoundEngine::RegisterGameObject(this);
			m_isInitialized = true;
		}
		else
		{
			EE_CORE_ERROR("Object '%s' must be linked with an existing scene!", m_name);
		}
	}
	
	//GameObject::~GameObject()
	//{
	//	if (m_scene)
	//	{
	//		auto& registryMap = GetRegistryMap();
	//		auto registryIt = registryMap.find(m_scene->m_registryId);

	//		if (registryIt == registryMap.end())
	//		{
	//			return;
	//		}
	//		if (!registryIt->second)
	//		{
	//			return;
	//		}

Elevate::GameObject::~GameObject()
{
	SoundEngine::UnregisterGameObject(this);

	//if (m_scene)
	//{
	//	m_scene->m_registryId.destroy(entt::entity(m_entityId));
	//} else EE_CORE_ERROR("Object '{0}' must be destroyed from an existing scene!", m_name);
}

	std::vector<Component*> GameObject::GetComponents()
	{
		std::vector<Component*> components;
		if (!m_scene) return components;

		for (auto& [type, entry] : ComponentRegistry::GetEntries()) {
			if (Component* component = entry.getter(weak_from_this())) {
				components.push_back(component);
			}
		}

		return components;
	}

	void GameObject::RenderInEditor()
	{
		// TODO MAKE GETCOMPONENTS ONLY RETURN ACTIVE COMOPNENTS TO PREVENT THE CHECK
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->RenderInEditor();
			}
		}

		for (const auto& child : m_childs)
		{
			child->RenderInEditor();
		}
	}

	void GameObject::RenderWhenSelected()
	{
		// TODO MAKE GETCOMPONENTS ONLY RETURN ACTIVE COMOPNENTS TO PREVENT THE CHECK
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->RenderWhenSelected();
			}
		}

		for (const auto& child : m_childs)
		{
			child->RenderWhenSelected();
		}
	}

	void GameObject::SetParent(const EEObjectPtr<GameObject>& newParent)
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
		}
		else {
			m_scene->AddRootObject(shared_from_this());
		}
	}

	void GameObject::Destroy()
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

	void GameObject::AddChild(const EEObjectPtr<GameObject>& child)
	{
		if (child)
		{
			child->m_parent = shared_from_this();
			m_childs.emplace(child);
			m_scene->RemoveFromRoot(child);
		}
	}

	void GameObject::RemoveChild(const EEObjectPtr<GameObject>& child)
	{
		m_childs.erase(child);
	}

	std::shared_ptr<GameObject> GameObject::Create(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(name, scene, parent);
		obj->Initialize();
		obj->SetPosition({ 0.0f, 0.0f, 0.0f });
		return obj;
	}

	// TODO MOVE SOMEWHERE ELSE
	glm::mat4 GameObject::GenGlobalMatrix() const
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
}
