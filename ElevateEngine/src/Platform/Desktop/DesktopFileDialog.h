#pragma once
#ifndef EE_PLATFORM_WEB

#include <ElevateEngine/Files/FileDialog.h>

namespace Elevate
{
	class DesktropFileDialog : public FileDialogImpl
	{
    public:
        DesktropFileDialog() = default;

        virtual void OpenFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple) override;

        virtual void SaveFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription) override;

        virtual void SelectFolder(const std::string& title, const std::string& defaultPath) override;

    private:
        std::vector<const char*> ConvertFilters(const std::vector<std::string>& filters) const;
	};
}

#endif