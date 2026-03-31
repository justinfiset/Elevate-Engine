#pragma once

#include <string>

namespace EL
{
	struct ProjectCreationProps {
		std::string Name = "NewProject";
		std::string Path = ".";
		bool bUsesWwise = false;
	};

	class ProjectManager
	{
		std::string m_lastMessage;

	public:
		bool CreateNewProject(const ProjectCreationProps& props) { return true; } // todo create;
		std::string GetLastMessage();
	};
}