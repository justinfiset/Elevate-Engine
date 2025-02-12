#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Elevate
{
	class Material;
	using MaterialPtr = std::shared_ptr<Material>;

	class Material
	{
	public:
		Material() :
			m_Ambient(1.0f, 1.0f, 1.0f),
			m_Diffuse(1.0f, 1.0f, 1.0f),
			m_Specular(1.0f, 1.0f, 1.0f),
			m_Shininess(128.0f)
		{ }

		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
			m_Ambient(ambient),
			m_Diffuse(diffuse),
			m_Specular(specular),
			m_Shininess(shininess)
		{ }

		inline glm::vec3& GetAmbiant() { return m_Ambient; }
		inline glm::vec3& GetDiffuse() { return m_Diffuse; }
		inline glm::vec3& GetSpecular() { return m_Specular; }
		inline float GetShininess() { return m_Shininess; }

	public:
		inline static MaterialPtr Create() { return std::make_shared<Material>(); }
		inline static MaterialPtr Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) { return std::make_shared<Material>(ambient, diffuse, specular, shininess); }

		// todo setter and make getter const (completly wrap the material)
	private:
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;
	};
}