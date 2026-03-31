#include "ProjectManager.h"

#include "../Models/Project.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace EL
{
	ProjectManager::ProjectManager()
	{
		RefreshProjectList();
	}

	bool ProjectManager::CreateNewProject(const ProjectCreationProps& props)
	{
		Project project;
		project.Name = props.Name;
		project.Path = props.Path;
		project.Id = m_projectList.size();
		project.UsesWwise = props.bUsesWwise;
		project.IsValid = IsProjectValid(project);
		m_projectList.push_back(project);
		return true;
	}

	void ProjectManager::RefreshProjectList()
	{
		// todo get info from local files
	}

	void ProjectManager::UpdateLocalProjectList()
	{
		// todo save info to local files
	}

	bool ProjectManager::IsProjectValid(const Project& project) const
	{
		if (!fs::exists(project.Path) || !fs::is_directory(project.Path))
		{
			return false;
		}

		for (const auto& entry : fs::directory_iterator(project.Path)) {
			if (entry.is_regular_file() && entry.path().extension() == ".eproj") {
				return true;
			}
		}
		return false;
	}

	const std::vector<Project>& ProjectManager::GetProjectList()
	{
		for (auto& project : m_projectList)
		{
			project.IsValid = IsProjectValid(project);
		}

		return m_projectList;
	}

	void ProjectManager::RemoveProjectFromList(uint32_t projectId)
	{
		auto it = std::find_if(m_projectList.begin(), m_projectList.end(), [projectId](const Project& project) {
			return project.Id == projectId;
		});

		if (it != m_projectList.end())
		{
			m_projectList.erase(it);
			UpdateLocalProjectList();
		}
	}

	std::string ProjectManager::GetLastMessage()
	{
		std::string temp = std::move(m_lastMessage);
		m_lastMessage = "";
		return temp;
	}
}