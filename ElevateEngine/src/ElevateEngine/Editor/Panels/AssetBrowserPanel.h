#pragma once
#include <filesystem>

#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Editor/EditorWidget.h>

namespace Elevate::Editor {
    
    enum FileType {
        Directory,
        File,
        Internal,
        Image
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
            else if (typeStr == "IMAGE") {
                return FileType::Image;
            }
            else {
                return FileType::Internal;
            }
        }
    };

    struct FileItem {
        std::string name;
        std::string path;
        std::string extension;
        std::string iconPath;
        FileType type;

        FileItem() = default;
        FileItem(std::string filePath, std::string fileName, std::string fileExtension, std::string icon, FileType type) :
            path(filePath), name(fileName), extension(fileExtension), iconPath(icon), type(type) { }
    };

    class AssetBrowserPanel : public EditorWidget
    {
    public:
        AssetBrowserPanel();

        void OnUpdate() override;
        void OnImGuiRender() override;

    private:
        void LoadFileItemsList();
        void LoadExtensionsMeta(std::string filepath = "editor/config/file_browser.json");

        std::filesystem::path m_CurrentPath = ".";

        std::vector<FileItem> m_FileItems;
        std::unordered_map<std::string, FileMetadata> m_FileMetadata;

        bool m_shouldUpdate = true;
    };
}