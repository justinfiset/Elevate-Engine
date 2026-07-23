#pragma once

#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <ElevateEngine/Core/Data.h>

#include <ElevateEngine/Core/ITypeLayoutProvider.h>
#include <ElevateEngine/Core/TypeLayout.h>

namespace Elevate
{
	class Transform : public ITypeLayoutProvider
	{
	public:
		Transform();

		// TODO mettre dans le fichier cpp
		void SetPosition(glm::vec3 pos) { this->position = pos; m_isDirty = true; }
		void SetRotation(glm::vec3 rot) { this->rotation = rot; m_isDirty = true; }
		void SetScale(glm::vec3 scale) { this->scale = scale; m_isDirty = true; }

		glm::vec3& GetPosition() { return position; }
		const glm::vec3& GetPosition() const { return position; }
		glm::vec3& GetRotation() { return rotation; }
		const glm::vec3& GetRotation() const { return rotation; }
		glm::vec3& GetScale() { return scale; }
		const glm::vec3& GetScale() const { return scale; }

		glm::vec3 GetRight() const;
		// TODO GETLEFT (-right)
		glm::vec3 GetUp() const;
		// TODO GETDOWN(); (-up)
		glm::vec3 GetBackward() const;
		glm::vec3 GetForward() const;
		glm::vec3 GetGlobalScale() const;

		const glm::mat4& GetModelMatrix() const;
		void UpdateModelMatrix();
	
		virtual TypeLayout GetLayout() const override
		{
			return TypeLayout(
				"Transform",
				{
					{"Positon", EngineDataType::Float3, &position},
					{"Rotation", EngineDataType::Float3, &rotation},
					{"Scale", EngineDataType::Float3, &scale}
				}
			);
		}
	private:
		bool m_isDirty = true;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::mat4 m_ModelMatrix;
	};
}