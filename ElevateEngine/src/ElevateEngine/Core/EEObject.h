#pragma once

#include <string>
#include <glm/vec4.hpp>

// Category class declaration and macro definition
class EECategory
{
public:
	EECategory() : m_path("") { }
	EECategory(const std::string& path) : m_path(path) { }

	inline const std::string GetName() 
	{ 
		size_t i = m_path.find_last_of('/');
		if (i == std::string::npos)
		{
			return m_path;
		}
		return m_path.substr(i + 1); 
	}

	bool operator<(const EECategory& other) const
	{
		return m_path < other.m_path;
	}

	glm::vec4 GetCategoryColor() {
		if (m_path == "Physics")  return glm::vec4(0.3f, 0.6f, 0.3f, 1.0f);   // Dark Vibrant Green
		if (m_path == "Render")   return glm::vec4(0.7f, 0.4f, 0.2f, 1.0f);   // Dark Vibrant Orange
		if (m_path == "Audio")    return glm::vec4(0.3f, 0.5f, 0.8f, 1.0f);   // Dark Vibrant Blue
		if (m_path == "Script")   return glm::vec4(0.6f, 0.3f, 0.7f, 1.0f);   // Dark Vibrant Purple
		return glm::vec4(0.2f, 0.3f, 0.4f, 1.0f);                             // Dark Blue-Grey for Default
	}

private:
	const std::string m_path;
};
#define EECATEGORY(name) virtual EECategory GetCategory() override { return EECategory(##name); }

class EEObject
{
public:
	virtual EECategory GetCategory() { return EECategory(); }
};