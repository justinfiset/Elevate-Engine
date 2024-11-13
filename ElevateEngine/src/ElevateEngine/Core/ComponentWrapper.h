#pragma once

#include <memory>
#include "Component.h"
#include "ElevateEngine/Core/Log.h"

// TODO deapreciate????? will it be useful for scrippted components??
namespace Elevate
{
    struct ComponentWrapper 
    {
        ComponentWrapper() = default;

        inline void SetGameObject(GameObject* gO)
        {
            component->gameObject = gO;
        }

        template<typename T, typename... Args>
        inline void SetComponent(Args&&... args) 
        {
            component = std::make_shared<T>(std::forward<Args>(args)...);
        }

        inline bool IsActive() { return component->m_IsActive; }

        inline void Init() { component->Init(); }
        inline void Destroy() { component->Destroy(); }
        inline void Update() { component->Update(); }
        inline void Render() { component->Render(); }
        inline void OnNotify(Event& e) { component->OnNotify(e); }

        // TODO trouver le moyen de mettre unique
        std::shared_ptr<Component> component;
    };
}