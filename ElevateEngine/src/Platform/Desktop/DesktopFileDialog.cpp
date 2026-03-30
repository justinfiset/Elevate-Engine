#include "DesktopFileDialog.h"

#ifndef EE_PLATFORM_WEB
#include <tinyfiledialogs.h>

namespace Elevate
{
	std::vector<const char*> DesktropFileDialog::ConvertFilters(const std::vector<std::string>& filters) const
	{
		std::vector<const char*> filterPtrs;
		filterPtrs.reserve(filters.size());
		for (const auto& f : filters)
		{
			filterPtrs.push_back(f.c_str());
		}
		return filterPtrs;
	}

	void DesktropFileDialog::OpenFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple)
	{
		std::vector<const char*> filterPtrs = ConvertFilters(filters);

		const char* result = tinyfd_openFileDialog(
			title.empty() ? nullptr : title.c_str(),
			defaultPathOrFile.empty() ? nullptr : defaultPathOrFile.c_str(),
			static_cast<int>(filterPtrs.size()),
			filterPtrs.empty() ? nullptr : filterPtrs.data(),
			filterDescription.empty() ? nullptr : filterDescription.c_str(),
			allowMultiple ? 1 : 0
		);

		if (result == nullptr)
		{
			result = "";
		}
		SetResult(std::string(result));
	}

	void DesktropFileDialog::SaveFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription)
	{
		std::vector<const char*> filterPtrs = ConvertFilters(filters);

		const char* result = tinyfd_saveFileDialog(
			title.empty() ? nullptr : title.c_str(),
			defaultPathOrFile.empty() ? nullptr : defaultPathOrFile.c_str(),
			static_cast<int>(filterPtrs.size()),
			filterPtrs.empty() ? nullptr : filterPtrs.data(),
			filterDescription.empty() ? nullptr : filterDescription.c_str()
		);

		if (result == nullptr)
		{
			result = "";
		}
		SetResult(std::string(result));
	}

	void DesktropFileDialog::SelectFolder(const std::string& title, const std::string& defaultPath)
	{
		const char* result = tinyfd_selectFolderDialog(
			title.empty() ? nullptr : title.c_str(),
			defaultPath.empty() ? nullptr : defaultPath.c_str()
		);

		if (result == nullptr)
		{
			result = "";
		}
		SetResult(std::string(result));
	}
}

#endif