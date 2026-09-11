#pragma once

#include <string>
#include <vector>

namespace Elevate
{
    class FileDialogImpl
    {
    public:
        FileDialogImpl() = default;
        virtual ~FileDialogImpl() = default;

        virtual void OpenFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple) = 0;

        virtual void SaveFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription) = 0;

        virtual void SelectFolder(const std::string& title, const std::string& defaultPath) = 0;

        virtual bool DisplayAndGetResult(std::string& outPath);

        bool HasResult();
        std::string ConsumeResult();

    protected:
        void SetResult(const std::string& result);

    private:
        bool m_hasResult = false;
        std::string m_result = "";
    };

    class FileDialog
    {
    public:
        static std::string RequestOpenFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription, bool allowMultiple);

        static std::string RequestSaveFile(const std::string& title, const std::string& defaultPathOrFile,
            const std::vector<std::string>& filters, const std::string& filterDescription);

        static std::string RequestSelectFolder(const std::string& title, const std::string& defaultPath);

        static bool DisplayAndGetResult(std::string& outPath);
    private:
        static bool ValidateImpl();

        static FileDialogImpl* s_impl;
    };
}