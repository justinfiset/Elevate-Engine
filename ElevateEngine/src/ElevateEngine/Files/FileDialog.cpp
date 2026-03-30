#include "FileDialog.h"
#include <ElevateEngine/Core/Core.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/PathResolver.h>

#ifndef EE_PLATFORM_WEB
    #include <Platform/Desktop/DesktopFileDialog.h>
    Elevate::FileDialogImpl* Elevate::FileDialog::s_impl = new Elevate::DesktropFileDialog();
#else
    #include <Platform/Web/WebFileDialog.h>
    Elevate::FileDialogImpl* Elevate::FileDialog::s_impl = new Elevate::WebFileDialog();
#endif

namespace Elevate
{
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
            s_impl->OpenFile(title, PathResolver::Resolve(defaultPathOrFile), filters, filterDescription, allowMultiple);
        }
        return "";
    }

    std::string FileDialog::RequestSaveFile(const std::string& title, const std::string& defaultPathOrFile, const std::vector<std::string>& filters, const std::string& filterDescription)
    {
        if (ValidateImpl())
        {
            s_impl->SaveFile(title, PathResolver::Resolve(defaultPathOrFile), filters, filterDescription);
        }
        return "";
    }

    std::string FileDialog::RequestSelectFolder(const std::string& title, const std::string& defaultPath)
    {
        if (ValidateImpl())
        {
            s_impl->SelectFolder(title, PathResolver::Resolve(defaultPath));
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
            EE_CORE_ERROR("FileDialog implementation not set!");
            return false;
        }
        return true;
    }
}