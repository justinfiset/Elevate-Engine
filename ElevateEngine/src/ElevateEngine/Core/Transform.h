#pragma once

#include <glm/glm.hpp>

namespace Elevate
{
	class Transform
	{
	public:
		Transform();

		void SetPosition(glm::vec3 pos) { this->position = pos; }
		void SetRotation(glm::vec3 rot) { this->rotation = rot; }
		void SetScale(glm::vec3 scale) { this->position = scale; }

		glm::vec3* GetPoition() { return &position; }
		glm::vec3* GetRotation() { return &rotation; }
		glm::vec3* GetScale() { return &scale; }

	public: // TODO change in the near future
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}