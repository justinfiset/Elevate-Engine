module;

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <glm/gtx/matrix_decompose.hpp> // iwyu: keep

module Elevate.Core.Scene;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;

import Elevate.Core.Types.TypeRegistry;
import Elevate.Core.Objects.ObjectPtr;

import Elevate.Audio.SoundEngine;

namespace Elevate
{
	uint32_t GameObject::s_goIdCount = 0;

	GameObject::GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
		: m_name(name), m_scene(scene.get()), m_parent(parent)
	{
		m_goId = s_goIdCount;
		s_goIdCount++;
		CoreLogger::Trace("Created object '{}', id='{}', GUID='{}'", m_name, m_goId, m_guid.ToString());
	}

	void GameObject::SetFromGlobalMatrix(const glm::mat4& newWorld)
	{
		glm::mat4 newLocal =
			m_parent
			? glm::inverse(m_parent->GenGlobalMatrix()) * newWorld
			: newWorld;

		glm::vec3 scale;
		glm::quat rotationQuat;
		glm::vec3 position;
		glm::vec3 skew;
		glm::vec4 perspective;

		if (!glm::decompose(newLocal, scale, rotationQuat, position, skew, perspective))
		{
			return;
		}
		rotationQuat = glm::normalize(rotationQuat);

		SetPosition(position);
		m_Transform.SetRotationQuaternion(rotationQuat);
		SetScale(scale);
	}

	glm::vec3 GameObject::GetGlobalPosition()
	{
		glm::mat4 global = GenGlobalMatrix();
		return glm::vec3(global[3]);
	}

	void GameObject::Awake()
	{
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->Awake();
			}
		}

		for (const auto& child : m_childs)
		{
			child->Awake();
		}
	}

	void GameObject::Start()
	{
		for (Component* comp : GetComponents())
		{
			if (comp->IsActive())
			{
				comp->Start();
			}
		}

		for (const auto& child : m_childs)
		{
			child->Start();
		}
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
		if (m_entityId != INVALID_ENTITY_ID && m_isInitialized)
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

	std::string GameObject::GetName() const
	{
		return m_name;
	}

	TypeLayout GameObject::GetLayout() const
	{
		std::vector<TypeField> allFields;

		// Add the guid
		allFields.push_back(::Elevate::TypeField("m_guid", EngineDataType::GUID, GetGuidOffset()));

		for (const ::Elevate::TypeField& field : generated_classEntry.ClassFieldStack)
		{
			allFields.push_back(field);
		}

		// Add all the components
		for (const auto& component : GetComponents())
		{
			const auto& layout = component->GetLayout();
			TypeField field;
			field.children.insert(std::end(field.children), layout.begin(), layout.end());
			field.name = layout.GetName();
			field.type = EngineDataType::Custom;

			allFields.push_back(field);
		}

		return TypeLayout(this, GetName(), allFields);
	}

	void GameObject::Initialize()
	{
		if (!m_scene)
		{
			CoreLogger::Error("Object {} must be linked with an existing scene!", m_name);
			return;
		}

		if (m_parent)
		{
			m_parent->AddChild(GetShared());
		}
		else
		{
			m_scene->AddRootObject(GetShared());
		}

		m_entityId = Registry().CreateEntity(m_scene->GetRegistryId());
		Assert::That(m_entityId != INVALID_ENTITY_ID, "Could not create a valid entity for GameObject : {}", m_name);

		SoundEngine::RegisterGameObject(this);
		m_isInitialized = true;
	}

	ComponentRegistry& GameObject::Registry() const
	{
		return m_scene->GetComponentRegistry();
	}

	uint32_t GameObject::RegistryId() const
	{
		return m_scene->GetRegistryId();
	}

	GameObject::~GameObject()
	{
		SoundEngine::UnregisterGameObject(this);

		if (m_scene && m_entityId != INVALID_ENTITY_ID)
		{
			Registry().DestroyEntity(m_scene->GetRegistryId(), m_entityId);
		}
	}

	std::shared_ptr<GameObject> GameObject::GetShared()
	{
		return std::static_pointer_cast<GameObject>(shared_from_this());
	}

	std::weak_ptr<GameObject> GameObject::GetWeak()
	{
		return std::static_pointer_cast<GameObject>(shared_from_this());
	}

	std::shared_ptr<const GameObject> GameObject::GetShared() const
	{
		return std::static_pointer_cast<const GameObject>(shared_from_this());
	}

	std::weak_ptr<const GameObject> GameObject::GetWeak() const
	{
		return std::static_pointer_cast<const GameObject>(shared_from_this());
	}

	std::vector<Component*> GameObject::GetComponents()
	{
		std::vector<Component*> components;

		if (!m_scene)
		{
			return components;
		}

		for (auto& [type, entry] : TypeRegistry::GetEntries())
		{
			if (auto* trait = entry.GetTrait<ComponentTypeTrait>())
			{
				if (trait->getter)
				{
					if (Component* component = static_cast<Component*>(trait->getter(this)))
					{
						components.push_back(component);
					}
				}
			}
		}

		return components;
	}

	std::vector<const Component*> GameObject::GetComponents() const
	{
		std::vector<const Component*> components;
		if (!m_scene)
		{
			return components;
		}

		for (auto& [type, entry] : TypeRegistry::GetEntries())
		{
			if (auto* trait = entry.GetTrait<ComponentTypeTrait>()) {
				if (trait->const_getter)
				{
					if (const Component* component = static_cast<const Component*>(trait->const_getter(this)))
					{
						components.push_back(component);
					}
				}
			}
		}

		return components;
	}

	void GameObject::RenderInEditor()
	{
		for (Component* comp : GetComponents())
		{
			comp->RenderInEditor();
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
			m_parent->RemoveChild(GetShared());
		}

		this->m_parent = newParent;

		if (newParent)
		{
			newParent->AddChild(GetShared());
			if (m_scene)
			{
				m_scene->RemoveFromRoot(GetShared());
			}
		}
		else {
			m_scene->AddRootObject(GetShared());
		}
	}

	void GameObject::Destroy()
	{
		if (m_parent) {
			m_parent->RemoveChild(GetShared());
		}
		else {
			m_scene->RemoveFromRoot(GetShared());
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
			child->m_parent = GetShared();
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
