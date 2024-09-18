#pragma once

#include "glm/glm.hpp"

namespace Elevate
{
	struct Vertex
	{
		glm::vec3 Position = { 0, 0, 0 };
		glm::vec3 Normal = { 0, 0, 0, };
		glm::vec2 TexCoords = { 0, 0 };
	};
}