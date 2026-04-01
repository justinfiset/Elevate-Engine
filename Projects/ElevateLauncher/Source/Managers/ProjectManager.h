#pragma once

#include <string>
#include <vector>

#include "../Models/Project.h"

namespace EL
{
	struct ProjectCreationProps {
		std::string Name = "NewProject";
		std::string TemplatePath = "";
		std::string Path = "";
		bool bUsesWwise = false;
	};

	class ProjectManager
	{
		std::string m_lastMessage = "";
		std::vector<Project> m_projectList;

		std::vector<ProjectTemplate> m_projectTemplates;
		bool m_templatesLoaded = false;

	public:
		ProjectManager();

		// Project Templates
		std::vector<ProjectTemplate> GetProjectTemplates();

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