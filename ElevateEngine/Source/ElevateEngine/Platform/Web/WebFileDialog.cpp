#include "WebFileDialog.h"

#ifdef EE_PLATFORM_WEB

#include <ImGuiFileDialog/ImGuiFileDialog.h>

namespace Elevate
{
	std::string WebFileDialog::ConvertFilters(const std::vector<std::string>& filters) const
	{
		size_t totalSize = 0;
		for (const auto& f : filters)
		{
			totalSize += f.length();
		}

		std::string filterString;
		filterString.reserve(totalSize);

		for (size_t i = 0; i < filters.size(); ++i)
		{
			filterString += filters[i].substr(1, filters[i].length() - 1); // Remove the leading "*""
			if (i < filters.size() - 1)
			{
				filterString += ",";
			}
		}

		return filterString;
	}


	void WebFileDialog::OpenFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple)
	{
		IGFD::FileDialogConfig config;
		config.path = defaultPathOrFile;
		config.flags = ImGuiFileDialogFlags_Modal;
		m_titleCache = title;
		ImGuiFileDialog::Instance()->OpenDialog(title, title, ConvertFilters(filters).c_str(), config);
		SetWaitingForFilePath();
	}

	void WebFileDialog::SaveFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription)
	{
		IGFD::FileDialogConfig config;
		config.path = defaultPathOrFile;
		config.flags = ImGuiFileDialogFlags_Modal;
		m_titleCache = title;
		ImGuiFileDialog::Instance()->OpenDialog(title, title, ConvertFilters(filters).c_str(), config);
		SetWaitingForFilePath();
	}

	void WebFileDialog::SelectFolder(const std::string& title, const std::string& defaultPath)
	{
		IGFD::FileDialogConfig config;
		config.path = defaultPath;
		config.flags = ImGuiFileDialogFlags_Modal;
		m_titleCache = title;
		ImGuiFileDialog::Instance()->OpenDialog(title, title, "", config);
		SetWaitingForDirPath();
	}

	bool WebFileDialog::DisplayAndGetResult(std::string& outPath)
	{
		// Display
		if (ImGuiFileDialog::Instance()->Display(m_titleCache, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Modal, ImVec2(600, 400))) {
			if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
				if (m_waitiingForFilePath)
				{
					SetResult(ImGuiFileDialog::Instance()->GetFilePathName());
				}
				else if (m_waitingForDirPath)
				{
					SetResult(ImGuiFileDialog::Instance()->GetCurrentPath());
				}
			}
			// close
			ImGuiFileDialog::Instance()->Close();
		}

		// Return result if possible
		if (HasResult())
		{
			m_isActive = false;
			outPath = ConsumeResult();
			return true;
		}
		return false;
	}

	void WebFileDialog::SetWaitingForDirPath()
	{
		m_isActive = true;
		m_waitingForDirPath = true;
		m_waitiingForFilePath = false;
	}

	void WebFileDialog::SetWaitingForFilePath()
	{
		m_isActive = true;
		m_waitingForDirPath = false;
		m_waitiingForFilePath = true;
	}
}

#endif