#pragma once
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate::Editor {
    
    enum FileType {
        Directory,
        File,
        Internal
    };

    struct FileMetadata {
        FileType type;
        std::string iconPath;

        FileMetadata() = default;
        FileMetadata(FileType type, std::string iconPath)
        {
            this->type = type;
            this->iconPath = iconPath;
        }

        static FileType ParseFileType(std::string typeStr) {
            if (typeStr == "DIRECTORY") {
                return FileType::Directory;
            }
            else if (typeStr == "FILE") {
                return FileType::File;
            }
            else {
                return FileType::Internal;
            }
        }
    };

    struct FileItem {
        std::string name;
        std::string path;
        FileMetadata metadata;
    };

    class AssetBrowserPanel
    {
    public:
        AssetBrowserPanel(std::string filepath = "editor/config/file_browser.json");

        void OnImGuiRender();

    private:
        std::unordered_map<std::string, FileMetadata> m_FileMetadata;
    };
}