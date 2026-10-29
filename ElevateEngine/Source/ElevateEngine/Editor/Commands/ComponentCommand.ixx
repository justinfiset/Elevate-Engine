module;

#include <memory>

export module Elevate.Editor.Commands.ComponentCommand;

// Foundations
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Scene;
import Elevate.Core.Types.TypeRegistry;
// Serialization
import Elevate.Serialization.PropertyField;
// Editor
import Elevate.Editor.Commands.Command;

export namespace Elevate
{
	class RemoveComponentCommand : public Command
	{
	public:
		RemoveComponentCommand(Component* comp)
		{
			if (!comp)
			{
				return;
			}

			m_obj = comp->gameObject->GetShared();
			m_factory = comp->GetFactory();
			m_destructor = comp->GetDestructor();
			m_properties = comp->GetProperties();
		}
	private:
		std::weak_ptr<GameObject> m_obj;
		GameObjectComponentFactory m_factory;
		GameObjectComponentDestructor m_destructor;
		PropertySet m_properties;

		virtual void Execute() override
		{
			if (auto go = m_obj.lock(); go && m_destructor)
			{
				m_destructor(go.get());
			}
		}

		virtual void Undo() override
		{
			if (m_factory)
			{
				if (auto go = m_obj.lock())
				{
					Component* created = static_cast<Component*>(m_factory(go.get()));
					if (created)
					{
						created->SetFromProperties(m_properties);
					}
				}
			}
		}
	};

	class AddComponentCommand : public Command
	{
	public:
		AddComponentCommand(
			std::weak_ptr<GameObject> object,
			GameObjectComponentFactory factory,
			GameObjectComponentDestructor destructor)
			: m_obj(object), m_factory(factory), m_destructor(destructor) {
		}
	private:
		std::weak_ptr<GameObject> m_obj;
		GameObjectComponentFactory m_factory;
		GameObjectComponentDestructor m_destructor;

		virtual void Execute() override
		{
			if (auto go = m_obj.lock(); go && m_factory)
			{
				m_factory(go.get());
			}
		}

		virtual void Undo() override
		{
			if (auto go = m_obj.lock(); go && m_destructor)
			{
				m_destructor(go.get());
			}
		}
	};
}
