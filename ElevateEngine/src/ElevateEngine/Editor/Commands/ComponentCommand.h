#pragma once

#include "eepch.h"
#include <ElevateEngine/Core/Command.h>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate
{
    class RemoveComponentCommand : public Command
    {
    public:
        RemoveComponentCommand(Component* comp)
            : m_component(comp) { }
    private:
        Component* m_component;
        virtual void Execute() override 
        { 
            if (m_component)
            {
                m_component->RemoveFromGameObject();
            }
        }
        virtual void Undo() override { /* TODO IMPLEMENT */ }
    };
}