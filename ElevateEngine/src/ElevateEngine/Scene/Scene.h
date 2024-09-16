#pragma once

#include <memory>
#include <set>
#include "ElevateEngine/Core/GameObject.h"
#include <entt/entt.hpp>

namespace Elevate
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		inline void AddRootObject(std::shared_ptr<GameObject> newRootObject) { m_RootObjects.insert(newRootObject); }
		const std::set<std::shared_ptr<GameObject>> GetRootObjects() const { return m_RootObjects; }
	private:
		entt::registry m_Registry;

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT
		std::set<std::shared_ptr<GameObject>> m_RootObjects;
	};
}