module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <cstdlib>
#include <iterator>
#include <new>
#include <string>
#include <vector>

module Elevate.Foundations.Files.FileDialog;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Paths;

namespace Elevate
{
	// C-linkage factories keep the platform implementation classes out of this
	// module's purview.  Otherwise Clang attaches their vtables to this module
	// here but emits them in the global module in the platform .cpp files,
	// leaving undefined module-qualified vtable symbols at link time.
	extern "C" FileDialogImpl* EE_CreateDesktopFileDialog();
	extern "C" FileDialogImpl* EE_CreateWebFileDialog();

#ifndef EE_PLATFORM_WEB
	FileDialogImpl* FileDialog::s_impl = EE_CreateDesktopFileDialog();
#else
	FileDialogImpl* FileDialog::s_impl = EE_CreateWebFileDialog();
#endif

	bool FileDialogImpl::DisplayAndGetResult(std::string& outPath)
	{
		if (HasResult())
		{
			outPath = ConsumeResult();
			return true;
		}
		return false;
	}

	bool FileDialogImpl::HasResult()
	{
		return m_hasResult;
	}

	std::string FileDialogImpl::ConsumeResult()
	{
		m_hasResult = false;
		return m_result;
	}

	void FileDialogImpl::SetResult(const std::string& result)
	{
		m_hasResult = true;
		m_result = result;
	}

	std::string FileDialog::RequestOpenFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple)
	{
		if (ValidateImpl())
		{
			s_impl->OpenFile(title, PathResolver::Resolve(defaultPathOrFile).string(), filters, filterDescription, allowMultiple);
		}
		return "";
	}

	std::string FileDialog::RequestSaveFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription)
	{
		if (ValidateImpl())
		{
			s_impl->SaveFile(title, PathResolver::Resolve(defaultPathOrFile).string(), filters, filterDescription);
		}
		return "";
	}

	std::string FileDialog::RequestSelectFolder(const std::string& title, const std::string& defaultPath)
	{
		if (ValidateImpl())
		{
			s_impl->SelectFolder(title, PathResolver::Resolve(defaultPath).string());
		}
		return "";
	}

	bool FileDialog::DisplayAndGetResult(std::string& outPath)
	{
		return s_impl->DisplayAndGetResult(outPath);
	}

	bool FileDialog::ValidateImpl()
	{
		if (!s_impl)
		{
			CoreLogger::Error("FileDialog implementation not set!");
			return false;
		}
		return true;
	}
}