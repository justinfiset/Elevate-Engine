#pragma once
#include "ElevateEngine/Core/ITransformable.h"

#include <memory>

namespace Elevate
{
	class Light : public ITransformable
	{
	public:
		// Getter and setters
		inline glm::vec3 SetAmbientColor(glm::vec3 color) { m_ambientColor = color; }
		inline glm::vec3 SetDiffuseColor(glm::vec3 color) { m_diffuseColor = color; }
		inline glm::vec3 SetSpecularColor(glm::vec3 color) { m_specularColor = color; }

		inline glm::vec3& GetAmbientColor() { return m_ambientColor; }
		inline glm::vec3& GetDiffuseColor() { return m_diffuseColor; }
		inline glm::vec3& GetSpecularColor() { return m_specularColor; }
	protected:
		glm::vec3 m_ambientColor;
		glm::vec3 m_diffuseColor;
		glm::vec3 m_specularColor;
	};
}