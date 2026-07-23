#include "ProjectManager.h"
#include <fstream>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Files/FileUtility.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Serialization/JsonSerializer.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace EL
{
	static constexpr std::string_view templatePath = "/Templates/";
	static constexpr std::string_view templateDescriptionFile = "description.txt";
	static constexpr std::string_view templateIconFile = "icon.png";

	ProjectManager::ProjectManager()
	{
		RefreshProjectList();
	}

	bool ProjectManager::CreateNewProject(const ProjectCreationProps& props)
	{
		// Create the project object and add it to the list
		Project project;
		project.Name = props.Name;
		project.Path = props.Path;
		project.Id = (uint32_t) m_projectList.Projects.size();
		project.UsesWwise = props.bUsesWwise;
		std::string projectDir = project.Path + "/" + project.Name;

		// Check if the given path is valid
		if (!fs::exists(props.Path) || !fs::is_directory(props.Path))
		{
			EE_ERROR("The provided path is not a valid directory. : {}", projectDir);
			m_lastMessage = "The provided path is not a valid directory.";
			return false;
		}

		if (fs::exists(projectDir))
		{
			EE_ERROR("A directory with this path already exists. : {}", projectDir);
			m_lastMessage = "A directory with this path already exists.";
			return false;
		}

		// Check if the project already exists
		if (IsProjectValid(project))
		{
			EE_ERROR("A project is already present in the folder.");
			m_lastMessage = "A project is already present in the folder.";
			return false;
		}

		// Create the project based on the template
		try
		{
			fs::create_directory(projectDir);
			fs::copy(props.TemplatePath + "/Assets/", projectDir, fs::copy_options::recursive | fs::copy_options::overwrite_existing);

			// Save the project config as a .eeproj
			Elevate::JsonSerializer serializer;
			Elevate::ByteBuffer bytes;
			serializer.Serialize(project.GetProperties(), bytes);

			std::string projectConfigPath = projectDir + "/" + project.Name + ".eeproj";
			std::ofstream outFile(projectConfigPath);

			if (outFile.is_open())
			{
				outFile << Elevate::ByteUtils::ToString(bytes);
				outFile.close();
			}
			else
			{
				EE_ERROR("Failed to write project config file at: {}", projectConfigPath);
				m_lastMessage = "Failed to create project configuration file.";
				return false;
			}
		}
		catch (fs::filesystem_error e)
		{
			EE_ERROR("Error Filesystem: {0}", e.what());
			EE_ERROR("Error code: {0}", e.code().value());
			m_lastMessage = std::string("FileSystem Error : ") + e.what();
			return false;
		}

		project.IsValid = IsProjectValid(project);
		if (!project.IsValid)
		{
			EE_ERROR("Failed to create a valid project.");
			return false;
		}

		m_projectList.Projects.push_back(project);
		UpdateLocalProjectList();
		return true;
	}

	std::vector<ProjectTemplate> ProjectManager::GetProjectTemplates()
	{
		if (m_templatesLoaded)
		{
			return m_projectTemplates;
		}

		std::string fullTemplateDir = EE_RESOURCE_DIR + std::string(templatePath);

		if (!fs::exists(fullTemplateDir) || !fs::is_directory(fullTemplateDir))
		{
			EE_WARN("Templates directory does not exist: {}", fullTemplateDir);
			m_templatesLoaded = true;
			return m_projectTemplates;
		}

		for (const auto& entry : fs::directory_iterator(fullTemplateDir)) {
			if (!entry.is_directory())
			{
				continue;
			}

			ProjectTemplate projTemplate;
			projTemplate.Path = entry.path().string();
			projTemplate.Name = entry.path().filename().string();

			std::string descriptionFile = projTemplate.Path + "/" + std::string(templateDescriptionFile);
			std::string thumbnailFile = projTemplate.Path + "/" + std::string(templateIconFile);

			if (fs::exists(descriptionFile))
			{
				projTemplate.Desription = Elevate::File::GetFileContent(descriptionFile);
			}
			if (fs::exists(thumbnailFile))
			{
				projTemplate.Thumbnail = Elevate::Texture::CreateFromFile(thumbnailFile);
			}
			m_projectTemplates.push_back(projTemplate);
		}

		m_templatesLoaded = true;
		return m_projectTemplates;
	}

	void ProjectManager::RefreshProjectList()
	{
		// todo get info from local files
	}

	void ProjectManager::UpdateLocalProjectList()
	{
		Elevate::JsonSerializer serializer;
		Elevate::ByteBuffer bytes;

		serializer.Serialize(m_projectList.GetProperties(), bytes);
		
		EE_INFO("Saving all of the following projects : ");
		std::string fileContent = Elevate::ByteUtils::ToString(bytes);
		EE_TRACE("{}", fileContent);

		std::string filePath = std::string(EE_CONTENT_ROOT) + "/projects.json";
		std::ofstream outFile(filePath);

		if (outFile.is_open())
		{
			outFile << fileContent;
			outFile.close();
		}
		else
		{
			EE_ERROR("Failed to write projects config list file at: {}", filePath);
		}
	}

	bool ProjectManager::IsProjectValid(const Project& project) const
	{
		std::string fullPath = project.Path + "/" + project.Name;
		if (!fs::exists(fullPath))
		{
			return false;
		}

		for (const auto& entry : fs::directory_iterator(project.Path + "/" + project.Name)) {
			if (entry.is_regular_file() && entry.path().extension() == ".eeproj") {
				return true;
			}
		}
		return false;
	}

	const std::vector<Project>& ProjectManager::GetProjectList()
	{
		for (auto& project : m_projectList.Projects)
		{
			project.IsValid = IsProjectValid(project);
		}

		return m_projectList.Projects;
	}

	void ProjectManager::RemoveProjectFromList(uint32_t projectId)
	{
		auto it = std::find_if(m_projectList.Projects.begin(), m_projectList.Projects.end(), [projectId](const Project& project) {
			return project.Id == projectId;
		});

		if (it != m_projectList.Projects.end())
		{
			m_projectList.Projects.erase(it);
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