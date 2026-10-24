module;

#include <memory>

export module Elevate.Editor.Commands.ComponentCommand;

// Foundations
import Elevate.Foundations.CoreLogger;
// Core
import Elevate.Core.Scene;
import Elevate.Core.Types.TypeRegistry;
// Editor
import Elevate.Editor.Commands.Command;

export namespace Elevate
{
	class RemoveComponentCommand : public Command
	{
	public:
		RemoveComponentCommand(Component* comp) : m_component(comp)
		{
			if (m_component)
			{
				try {
					m_obj = comp->gameObject->GetShared();
				}
				catch (...) {}

				m_backup.reset(m_component->Clone());
				m_factory = m_component->GetFactory();
				m_destructor = m_component->GetDestructor();
			}
		}
	private:
		std::weak_ptr<GameObject> m_obj;
		Component* m_component = nullptr;
		std::unique_ptr<Component> m_backup;
		GameObjectComponentFactory m_factory;
		GameObjectComponentDestructor m_destructor;

		virtual void Execute() override
		{
			if (auto go = m_obj.lock(); go && m_destructor)
			{
				m_destructor(go.get());
			}
		}

		virtual void Undo() override
		{
			if (m_backup && m_factory)
			{
				if (auto go = m_obj.lock())
				{
					Component* created = static_cast<Component*>(m_factory(go.get()));
					if (created)
					{
						created->CopyFrom(m_backup.get());
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
			CoreLogger::Trace("AddComponentCommand");
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
