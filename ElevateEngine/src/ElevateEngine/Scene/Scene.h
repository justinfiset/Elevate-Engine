#pragma once

#include <memory>
#include <entt/entt.hpp>

namespace Elevate
{
	class GameObject;

	class Scene
	{
	public:
		Scene();
		Scene(std::string name) : m_Name(name) {}
		~Scene();
		
		void UpdateScene();
		void RenderScene();

		inline const std::string GetName() const { return m_Name; };

		void AddRootObject(std::shared_ptr<GameObject> newRootObject);
		const std::set<std::shared_ptr<GameObject>> GetRootObjects() const { return m_RootObjects; }
	private:
		std::string m_Name;

		entt::registry m_Registry; // Component registry for all entt::entity

		// TODO ENLEVER ON VA REMPLACER PAR LE REGISTRY ENTT
		std::set<std::shared_ptr<GameObject>> m_RootObjects;

		friend class GameObject;
	};
}