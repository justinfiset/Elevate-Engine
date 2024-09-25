#pragma once

#include <memory>

namespace Elevate
{
	class Component;

    struct ComponentWrapper 
    {
        template<typename T, typename... Args>
        static ComponentWrapper& Create(Args&&... args)
        {
            ComponentWrapper wrapper;
            wrapper.component = std::make_unique<T>(std::forward<Args>(args)...);
            return wrapper;
        }

        // TODO trouver le moyen de mettre unique
        std::unique_ptr<Component> component;
    };
}