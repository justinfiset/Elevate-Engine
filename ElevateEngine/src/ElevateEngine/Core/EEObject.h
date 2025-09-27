#pragma once

#include <string>
#include <glm/vec4.hpp>

// Category class declaration and macro definition
class EECategory
{
public:
	EECategory() : EECategory("") { }
	EECategory(const std::string& path) :
		m_path(path), m_name(GenerateName()), m_depth(GenerateDepth()) { }
	EECategory(const EECategory& other)
		: m_depth(other.m_depth), m_name(other.m_name), m_path(other.m_path) { }

	inline const std::string& GetName() const
	{ 
		return m_name;
	}

	inline const std::string& GetPath() const
	{
		return m_path;
	}

	inline size_t GetDepth()
	{
		return m_depth;
	}

	bool operator<(const EECategory& other) const
	{
		return m_path < other.m_path;
	}

	bool operator==(const EECategory& other) const
	{
		return m_path == other.m_path;
	}

	glm::vec4 GetCategoryColor() {
		if (m_path == "Physics")  return glm::vec4(0.3f, 0.6f, 0.3f, 1.0f);   // Dark Vibrant Green
		if (m_path == "Rendering")   return glm::vec4(0.7f, 0.4f, 0.2f, 1.0f);   // Dark Vibrant Orange
		if (m_path == "Audio")    return glm::vec4(0.3f, 0.5f, 0.8f, 1.0f);   // Dark Vibrant Blue
		if (m_path == "Script")   return glm::vec4(0.6f, 0.3f, 0.7f, 1.0f);   // Dark Vibrant Purple
		if (m_path == "Transform") return glm::vec4(0.9f, 0.9f, 0.3f, 1.0f);  // Bright Yellow
		return glm::vec4(0.2f, 0.3f, 0.4f, 1.0f);                             // Dark Blue-Grey for Default
	}

	inline std::string GetNameAtDepth(size_t index) const
	{
		size_t start = 0;
		size_t end = m_path.find('/');

		for (size_t i = 0; i < index; i++)
		{
			if (end == std::string::npos)
			{
				return "";
			}
			start = end + 1;
			end = m_path.find('/', start);
		}

		if (end == std::string::npos)
		{
			return m_path.substr(start);
		}
		else
		{
			return m_path.substr(start, end - start);
		}
	}

private:
	inline std::string GenerateName() const
	{
		if (m_path.empty())
		{
			return "Default";
		}

		size_t i = m_path.find_last_of('/');
		if (i == std::string::npos)
		{
			return m_path;
		}

		return m_path.substr(i + 1);
	}

	inline size_t GenerateDepth() const
	{
		size_t count = 0;
		for (auto& c : m_path)
		{
			if (c == '/')
			{
				count++;
			}
		}
		return count;
	}
private:
	std::string m_path;
	std::string m_name;
	size_t m_depth;
};

class EEObject
{
protected:
	EECategory m_category;
public:
	virtual EECategory GetCategory() const { return m_category; }
};