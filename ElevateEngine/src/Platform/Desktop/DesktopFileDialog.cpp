#include "DesktopFileDialog.h"

#ifndef EE_PLATFORM_WEB
#include <tinyfiledialogs.h>

#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h>
#include <atlbase.h>
#endif

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
#ifdef _WIN32
        std::string selectedFolder = "";

        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        bool coInit = SUCCEEDED(hr);

        IFileOpenDialog* pFileOpen = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS options;
            if (SUCCEEDED(pFileOpen->GetOptions(&options)))
            {
                pFileOpen->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
            }

            if (!title.empty())
            {
                int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), (int)title.size(), nullptr, 0);
                std::wstring wTitle(sizeNeeded, 0);
                MultiByteToWideChar(CP_UTF8, 0, title.c_str(), (int)title.size(), &wTitle[0], sizeNeeded);
                pFileOpen->SetTitle(wTitle.c_str());
            }

            if (SUCCEEDED(pFileOpen->Show(nullptr)))
            {
                IShellItem* pItem = nullptr;
                if (SUCCEEDED(pFileOpen->GetResult(&pItem)))
                {
                    PWSTR pszPath = nullptr;
                    if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath)))
                    {
                        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, nullptr, 0, nullptr, nullptr);
                        std::string path(sizeNeeded, 0);
                        WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, &path[0], sizeNeeded, nullptr, nullptr);

                        if (!path.empty() && path.back() == '\0')
                        {
                            path.pop_back();
                        }
                        selectedFolder = path;
                        CoTaskMemFree(pszPath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }

        if (coInit)
        {
            CoUninitialize();
        }
        SetResult(selectedFolder);
#else
        const char* result = tinyfd_selectFolderDialog(
            title.empty() ? nullptr : title.c_str(),
            defaultPath.empty() ? nullptr : defaultPath.c_str()
        );

        if (result == nullptr)
        {
            result = "";
        }
        SetResult(std::string(result));
#endif
    }
}

#endif