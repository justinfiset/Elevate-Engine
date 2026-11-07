module;

#include "glm/glm.hpp"

export module Elevate.Renderer.Vertex;

export namespace Elevate
{
	struct Vertex
	{
		glm::vec3 Position = { 0, 0, 0 };
		glm::vec3 Normal = { 0, 0, 0, };
		glm::vec2 TexCoords = { 0, 0 };
		glm::vec3 Tangent = { 0, 0, 0 };
		glm::vec3 Bitangent = { 0, 0, 0 };
	};
}
