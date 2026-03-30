#pragma once

#include <string_view>

namespace EL
{
	class ProjectManager;

	enum class LauncherTab {
		ProjectList,
		ProjectCreation,
		Settings
	};

	class ProjectController
	{
	private:
		static constexpr std::string_view DocumentationURL = "https://justinfiset.github.io/Elevate-Engine/";

		ProjectManager& m_projectManager;

	public:
		LauncherTab m_ActiveTab = LauncherTab::ProjectList;

		ProjectController(ProjectManager& manager)
			: m_projectManager(manager) { }

		// Change launcher tab
		void SetActiveTab(LauncherTab tab) { m_ActiveTab = tab; }
		LauncherTab GetActiveTab() const { return m_ActiveTab; }

		void OpenSelectedProject() {} // todo impl
		void CreateNewProject() {} // todo impl

		void OpenDocumentation();

		void ReportIssue() {} // todo impl
		void RateEngine() {} // todo impl
	};
}