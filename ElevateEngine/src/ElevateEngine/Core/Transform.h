#pragma once

#include <memory>

#include <glm/mat4x4.hpp> // iwyu: keep
#include <glm/gtc/quaternion.hpp> // iwyu: keep
#include <glm/vec3.hpp> // iwyu: keep

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/ITypeLayoutProvider.h>
#include <ElevateEngine/Core/TypeLayout.h>

namespace Elevate
{
	class Transform : public ITypeLayoutProvider
	{
	public:
		Transform();

		void SetPosition(const glm::vec3& pos);
		void SetRotation(const glm::vec3& rot);
		void SetRotationQuaternion(const glm::quat& rot);
		void SetScale(const glm::vec3& scale);

		glm::vec3& GetPosition() { return position; }
		const glm::vec3& GetPosition() const { return position; }
		glm::vec3 GetRotation() const;
		const glm::quat& GetRotationQuat() const;
		glm::vec3& GetScale() { return scale; }
		const glm::vec3& GetScale() const { return scale; }

		glm::vec3 GetRight() const;
		glm::vec3 GetLeft() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetDown() const;
		glm::vec3 GetBackward() const;
		glm::vec3 GetForward() const;
		glm::vec3 GetGlobalScale() const;

		const glm::mat4& GetModelMatrix() const;
		void UpdateModelMatrix();
	
		virtual TypeLayout GetLayout() const override
		{
			return TypeLayout(
				this,
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

		glm::vec3 position{ 0.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f };

		glm::mat4 m_ModelMatrix;
	};
}