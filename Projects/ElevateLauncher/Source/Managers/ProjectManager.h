#pragma once

#include <string>
#include <vector>

namespace EL
{
	struct Project;

	struct ProjectCreationProps {
		std::string Name = "NewProject";
		std::string Path = ".";
		bool bUsesWwise = false;
	};

	class ProjectManager
	{
		std::string m_lastMessage = "";
		std::vector<Project> m_projectList;

	public:
		ProjectManager();

		// Project Managment
		bool CreateNewProject(const ProjectCreationProps& props);

		// Project List
		void RefreshProjectList();
		const std::vector<Project>& GetProjectList();
		void RemoveProjectFromList(uint32_t projectId);
		void UpdateLocalProjectList();
		bool IsProjectValid(const Project& project) const;

		std::string GetLastMessage();
	};
}