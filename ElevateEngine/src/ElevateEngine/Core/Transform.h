#pragma once

#include <glm/glm.hpp>

namespace Elevate
{
	class Transform
	{
	public:
		Transform();

		void SetPosition(glm::vec3 pos) { this->position = pos; }
		void SetRotation(glm::vec3 pos) { this->position = pos; }
		void SetScale(glm::vec3 pos) { this->position = pos; }

		const glm::vec3 GetPoition() { return this->position; }
		const glm::vec3 GetRotation() { return this->rotation; }
		const glm::vec3 GetScale() { return this->scale; }

	public: // TODO change in the near future
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}