module;

#include <functional>
#include <memory>

export module Elevate.Core.Scene.ComponentTypeTrait;

import Elevate.Core.Objects.Category;
import Elevate.Core.Types.ITypeTrait;

export namespace Elevate
{
	using GameObjectComponentGetter = std::function<void* (void*)>;
	using GameObjectConstComponentGetter = std::function<const void* (const void*)>;
	using GameObjectComponentFactory = std::function<void* (void*)>;
	using GameObjectComponentDestructor = std::function<void(void*)>;

	struct ComponentTypeTrait : public ITypeTrait
	{
		EECategory category;
		GameObjectComponentGetter getter; // method to get the type of component from a gameobject
		GameObjectConstComponentGetter const_getter;
		GameObjectComponentFactory factory; // factory to create / add to a gameObject
		GameObjectComponentDestructor destructor; // component destructor / remove from a gameObject
	};
}
