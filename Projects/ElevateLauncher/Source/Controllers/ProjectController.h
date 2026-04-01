#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <cstdint>
#include <future>

namespace EL
{
	struct Project;
	struct ProjectTemplate;
	struct ProjectCreationProps;
	class ProjectManager;

	struct Notification
	{
		enum class MsgType
		{
			Success,
			Info,
			Warning,
			Error
		};

		uint16_t Id;
		std::string Message;
		MsgType Type;
	};

	enum class LauncherTab
	{
		ProjectList,
		ProjectCreation,
		Settings
	};

	class ProjectController
	{
	private:
		static constexpr std::string_view DocumentationURL	= "https://justinfiset.github.io/Elevate-Engine/";
		static constexpr std::string_view IssueReportURL	= "https://github.com/justinfiset/Elevate-Engine/issues";
		static constexpr std::string_view RateEngineURL = "https://github.com/justinfiset/Elevate-Engine/discussions/128";

		ProjectManager& m_manager;
		LauncherTab m_activeTab = LauncherTab::ProjectList;

		std::vector<Notification> m_notifications;
		uint16_t m_nextNotificationId = 0;

		std::future<bool> m_creationFuture;
		bool m_isCreating = false;
	public:
		ProjectController(ProjectManager& manager)
			: m_manager(manager) { }

		// Loading
		bool IsLoading();
		void UpdateLoading();

		// Change launcher tab
		void SetActiveTab(LauncherTab tab);
		LauncherTab GetActiveTab() const;

		// Project Templates
		std::vector<ProjectTemplate> GetProjectTemplates() const;

		// Project Managment
		void OpenSelectedProject() {} // todo impl
		bool CreateNewProject(const ProjectCreationProps& props);
		const std::vector<Project>& GetProjectList();
		void RemoveProjectFromList(uint32_t projectId);

		// Navigation
		void OpenDocumentation() const;
		void ReportIssue() const;
		void RateEngine() const;

		// Notification System
		void PushNotification(const std::string& msg, Notification::MsgType type);
		void RemoveNotification(uint16_t id);
		const std::vector<Notification>& GetNotifications();

	private:
		void SetLoadingState(bool newState);
		void PushLastManagerError();
	};
}