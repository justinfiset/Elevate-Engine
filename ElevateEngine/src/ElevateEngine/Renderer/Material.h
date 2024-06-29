#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "ElevateEngine/Renderer/Shader.h"

namespace Elevate
{
	class Material
	{
	public:
		Material() :
			m_Ambient(1.0f, 1.0f, 1.0f),
			m_Diffuse(1.0f, 1.0f, 1.0f),
			m_Specular(1.0f, 1.0f, 1.0f),
			m_Shininess(128.0f)
		{}

		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
			m_Ambient(ambient),
			m_Diffuse(diffuse),
			m_Specular(specular),
			m_Shininess(shininess)
		{ }

		const void Use(std::shared_ptr<Shader> shader) const;
	private:
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;
	};
}