#pragma once

#ifdef EE_PLATFORM_WEB
#include <ElevateEngine/Files/FileDialog.h>

namespace Elevate
{
	class WebFileDialog : public FileDialogImpl
	{
    public:
        WebFileDialog() = default;

        virtual void OpenFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple) override;

        virtual void SaveFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription) override;

        virtual void SelectFolder(const std::string& title, const std::string& defaultPath) override;

    protected:
        virtual bool DisplayAndGetResult(std::string& outPath) override;

        void SetWaitingForDirPath();
        void SetWaitingForFilePath();

    private:
        std::string m_titleCache = "";
        bool m_isActive = false;
        bool m_waitingForDirPath = false;
        bool m_waitiingForFilePath = false;

        std::string ConvertFilters(const std::vector<std::string>& filters) const;
	};
}

#endif