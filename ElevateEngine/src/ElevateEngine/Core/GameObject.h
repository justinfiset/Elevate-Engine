#pragma once

#include "ITransformable.h"
#include <vector>
#include <entt/entt.hpp>
#include "ElevateEngine/Scene/Scene.h"
#include "ElevateEngine/Core/ComponentWrapper.h"
#include "ElevateEngine/Core/Component.h"

#define EE_VALIDATE_COMPONENT_TYPE() EE_CORE_ASSERT((std::is_base_of<Component, T>::value), "{0} : Type specifier must be a child of the Component class.", m_Name);

// TODO REMOVE ASAP ONCE NOT NEEDED IN THE FILE (header)
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
/////// ^^^^^^ TO REMOVE ASAP

namespace Elevate
{

	class GameObject : public ITransformable, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(std::string name, ScenePtr scene, GameObjectPtr parent = nullptr);
		~GameObject();

		// TODO REMOVE USELESS CODE (IN COMMENTS)
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EE_VALIDATE_COMPONENT_TYPE();

		    ComponentWrapper& emplacedWrapper = m_Scene->m_Registry.emplace<ComponentWrapper>(m_Entity);
			emplacedWrapper.SetComponent<T>(std::forward<Args>(args)...);
			emplacedWrapper.SetGameObject(this);
			emplacedWrapper.Init();

			T& component = static_cast<T&>(*emplacedWrapper.component.get());
			return component;
			//EE_VALIDATE_COMPONENT_TYPE();

			//T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);

			//Component& baseComponent = static_cast<Component&>(component);
			//baseComponent.gameObject = this;
			//baseComponent.Init();

			//return component;

			// anciennec version
			/*try
			{
				ComponentWrapper& tempWrapper = ComponentWrapper::Create<T>(std::forward<Args>(args)...);

				tempWrapper.SetGameObject(this);
				tempWrapper.Init();
				
				ComponentWrapper& emplacedWrapper = m_Scene->m_Registry.emplace_or_replace<ComponentWrapper>(m_Entity, std::move(tempWrapper));
				T& component = static_cast<T&>(*emplacedWrapper.component);

				EE_CORE_TRACE(m_Scene->GetName());
				return component;
			}
			catch (...) 
			{
				EE_CORE_ERROR("{0} : Caught an unknown excpetion while adding a component.", m_Name);
			}*/
		}

		template <typename T>
		T* GetComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			T* foundComponent = nullptr;
			m_Scene->m_Registry.view<ComponentWrapper>().each([&foundComponent](auto& wrapper)
			{
				if (wrapper.component && typeid(*wrapper.component) == typeid(T))
				{
					foundComponent = static_cast<T*>(wrapper.component.get());
				}
			});

			if (!foundComponent)
			{
				EE_CORE_TRACE("{} : Trying to get a missing component. You need to add the component before retrieving it.", m_Name);
			}

			return foundComponent;
		}

		template <typename T>
		bool HasComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			return m_Scene->m_Registry.all_of<T>(m_Entity);
		}

		template <typename T>
		void RemoveComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			if (HasComponent<T>()) m_Scene->m_Registry.remove<T>(m_Entity);
			else EE_CORE_ERROR("Trying to remove a missing component. You need to add the component before removing it.");
		}

		inline std::string& GetName() { return m_Name; }
		inline void SetName(std::string newName) { m_Name = newName; }
		
		void SetParent(GameObjectPtr newParent);

		// TODO SET LE PARENT DU CHILD COMME ÉTANT THIS
		void SetChild(GameObjectPtr child);
		void RemoveChild(GameObjectPtr child);

		inline const bool HasChild() const { return m_Childs.size() > 0; }
		inline std::set<GameObjectPtr> GetChilds() const { return m_Childs; }
		 
		// TODO MOVE TO ANOTHER FILE
		glm::mat4 GenGlobalMatrix() const;

		static GameObjectPtr Create(std::string name, ScenePtr scene, GameObjectPtr parent = nullptr);

		// TOdo mettre dans une autre fichier / classe
		inline void SetFromGlobalMatrix(const glm::mat4& newWorld)
		{
			glm::mat4 newLocal;
			if (m_Parent)
			{
				newLocal = glm::inverse(m_Parent->GenGlobalMatrix()) * newWorld;
			}
			else
			{
				newLocal = newWorld;
			}

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			ImGuizmo::DecomposeMatrixToComponents
            (
                glm::value_ptr(newLocal),
                glm::value_ptr(position),
                glm::value_ptr(rotation),
                glm::value_ptr(scale)
            );

            // TODO SHOULD SET GLOBAL INSTEAD OF LOCAL, THEREFORE THESE DO NOT ALWYAS WORKS ON CHILDS FOR THE MOMENT
            // -> IL FAUDRAIT UNE MÉTHODE QUI UPDATE LE TOUT À PARTIR D'UNE MÉTHODE SetFromGlobalMatrix() qui set le tout
			SetScale(scale);
            SetRotation(rotation);
            SetPosition(position);
		}

		// TODO SET GETTERS ELSEWHERE ASAP AND OPTIMIZE
		inline glm::vec3 GetGlobalPosition()
		{
			glm::mat4 global = GenGlobalMatrix();
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			ImGuizmo::DecomposeMatrixToComponents
			(
				glm::value_ptr(global),
				glm::value_ptr(position),
				glm::value_ptr(rotation),
				glm::value_ptr(scale)
			);

			return position;
		}
	private :
		void Initialize(); // Internal function to use just after constructor

		// TODO MAYBE REMOVE AND FIND A BETTER STRUCTURE
		// Transforms callbakcs
		virtual void OnSetPosition() override;


	private:
		std::string m_Name;

		// Parent and Child
		GameObjectPtr m_Parent;
		std::set<GameObjectPtr> m_Childs;

		entt::entity m_Entity { entt::null };
		Scene* m_Scene;

		friend class Scene;
	};
}