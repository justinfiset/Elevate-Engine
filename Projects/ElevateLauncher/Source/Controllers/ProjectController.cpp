#include "ProjectController.h"
#include "../Managers/ProjectManager.h"

#include <algorithm>
#include <ElevateEngine/Core/Platform.h>

namespace EL
{
	bool ProjectController::IsLoading()
	{
		return m_isCreating;
	}

	void ProjectController::UpdateLoading()
	{
		if (m_isCreating && m_creationFuture.valid())
		{
			if (m_creationFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				bool success = m_creationFuture.get();
				m_isCreating = false;
				if (!success)
				{
					PushLastManagerError();
				}
				else
				{
					SetActiveTab(LauncherTab::ProjectList);
					PushNotification("Successfully created project.", Notification::MsgType::Success);
				}
			}
		}
	}

	void ProjectController::SetActiveTab(LauncherTab tab)
	{
		m_activeTab = tab;
		m_notifications.clear();
	}

	LauncherTab ProjectController::GetActiveTab() const
	{
		return m_activeTab;
	}

	std::vector<ProjectTemplate> ProjectController::GetProjectTemplates() const
	{
		return m_manager.GetProjectTemplates();
	}

	bool ProjectController::CreateNewProject(const ProjectCreationProps& props)
	{
		m_notifications.clear();

		if (props.Name.empty())
		{
			PushNotification("You must enter a valid name for the project.", Notification::MsgType::Error);
			return false;
		}

		if (props.Path.empty())
		{
			PushNotification("You must enter a valid path for the project.", Notification::MsgType::Error);
			return false;
		}

		if (m_isCreating)
		{
			PushNotification("A project is already in creation. Please try again later.", Notification::MsgType::Error);
			return false;
		}

		m_isCreating = true;
		m_creationFuture = std::async(std::launch::async, [this, props]() {
			return m_manager.CreateNewProject(props);
		});

		return true;
	}

	const std::vector<Project>& ProjectController::GetProjectList()
	{
		return m_manager.GetProjectList();
	}

	void ProjectController::RemoveProjectFromList(uint32_t projectId)
	{
		m_manager.RemoveProjectFromList(projectId);
	}

	void ProjectController::OpenDocumentation() const
	{
		Elevate::Platform::OpenURL(std::string(DocumentationURL));
	}

	void ProjectController::ReportIssue() const
	{
		Elevate::Platform::OpenURL(std::string(IssueReportURL));
	}

	void ProjectController::RateEngine() const
	{
		Elevate::Platform::OpenURL(std::string(RateEngineURL));
	}

	void ProjectController::PushNotification(const std::string& msg, Notification::MsgType type)
	{
		if (msg.empty()) return;
		Notification notification;
		notification.Id = m_nextNotificationId++;
		notification.Message = msg;
		notification.Type = type;
		m_notifications.push_back(notification);
	}

	void ProjectController::RemoveNotification(uint16_t id)
	{
		auto it = std::find_if(m_notifications.begin(), m_notifications.end(), [id](const Notification& n) {
			return n.Id == id;
		});

		if (it != m_notifications.end())
		{
			m_notifications.erase(it);
		}
	}

	const std::vector<Notification>& ProjectController::GetNotifications()
	{
		return m_notifications;
	}

	void ProjectController::PushLastManagerError()
	{
		PushNotification(m_manager.GetLastMessage(), Notification::MsgType::Error);
	}
}
