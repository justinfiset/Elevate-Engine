#pragma once

#include <iostream>
#include "ITransformable.h"
#include <vector>

namespace Elevate
{
	class GameObject : public ITransformable
	{
	public:
		GameObject(std::string name) : m_Name(name) { }
		~GameObject() { }

		inline std::string& GetName() { return m_Name; }
		inline void SetName(std::string newName) { m_Name = newName; }

		inline std::vector<std::shared_ptr<GameObject>> GetChilds() const { return m_Childs; }
		
		// TODO AddChild
		//void AddChild(std::shared_ptr<GameObject> newObject);
		
		// TODO getter and setters for components
	private:
		std::string m_Name;

		// Parent and Child
		std::shared_ptr<GameObject> m_Parent;
		std::vector<std::shared_ptr<GameObject>> m_Childs;

		// TODO include soon
		//std::vector<Component> m_ComponentList;
	};
}