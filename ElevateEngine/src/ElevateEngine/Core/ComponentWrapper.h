#pragma once

#include <memory>
#include "Component.h"
#include "ElevateEngine/Core/Log.h"

// TODO deapreciate????? will it be useful for scrippted components??
namespace Elevate
{
    struct ComponentWrapper 
    {
        template<typename T, typename... Args>
        static ComponentWrapper Create(Args&&... args)
        {
            ComponentWrapper wrapper;
            wrapper.component = std::make_unique<T>(std::forward<Args>(args)...);
            return wrapper;
        }

        inline void SetGameObject(GameObject* gO)
        {
            component->gameObject = gO;
        }

        inline void Init() { component->Init(); }
        inline void Destroy() { component->Destroy(); }
        inline void Update() { component->Update(); }
        inline void Render() { component->Render(); }
        // TODO trouver le moyen de mettre unique
        std::unique_ptr<Component> component;
    };
}