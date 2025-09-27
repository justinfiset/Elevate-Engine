#pragma once

#include "eepch.h"
#include <ElevateEngine/Core/Command.h>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>
#include "ElevateEngine/Core/ComponentRegistry.h"

namespace Elevate
{
    class RemoveComponentCommand : public Command
    {
    public:
        RemoveComponentCommand(Component* comp) : m_component(comp) 
        {
            if (m_component)
            {
                try {
                    m_obj = comp->gameObject->shared_from_this();
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
            if (!m_obj.expired() && m_destructor) {
                m_destructor(m_obj);
            }
        }

        virtual void Undo() override 
        {
            if (m_backup && !m_obj.expired() && m_factory)
            {
                Component* created = m_factory(m_obj);
                if (created)
                {
                    created->CopyFrom(m_backup.get());
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
            : m_obj(object), m_factory(factory), m_destructor(destructor) { }
    private:
        std::weak_ptr<GameObject> m_obj;
        GameObjectComponentFactory m_factory;
        GameObjectComponentDestructor m_destructor;

        virtual void Execute() override
        {
            EE_CORE_TRACE("AddComponentCommand");
            if (!m_obj.expired() && m_factory)
            {
                m_factory(m_obj);
            }
        }

        virtual void Undo() override
        {
            if (!m_obj.expired() && m_destructor)
            {
                m_destructor(m_obj);
            }
        }
    };
}