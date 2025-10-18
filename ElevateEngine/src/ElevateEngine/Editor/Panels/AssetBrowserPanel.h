#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>

#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate::Editor {
    
    enum FileType {
        Directory = 0,
        File = 1,
        Internal = 2,
        Image = 3
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
        FileType type = File;

        FileItem() = default;
        FileItem(std::string filePath, std::string fileName, std::string fileExtension, std::string icon, FileType type) :
            path(filePath), name(fileName), extension(fileExtension), iconPath(icon), type(type) { }
    };

    struct BrowserPath {
        std::filesystem::path Path;
        std::string DisplayName;
    };

    class AssetBrowserPanel : public EditorWidget
    {
    public:
        AssetBrowserPanel();

        void OnUpdate() override;
        void OnImGuiRender() override;

    private:
        // Update the paths at the top of the browser
        void UpdateRelatedPaths();
        void AddParentPaths(std::filesystem::path path);

        void LoadFileItemsList();
        void LoadExtensionsMeta(std::string filepath = "editor/config/file_browser.json");

        std::filesystem::path m_CurrentPath = ".";
        std::vector<BrowserPath> m_relatedPaths;

        //      < Path , Texture >
        std::unordered_map<std::string, TexturePtr> m_currentTextures;
        std::vector<std::string> m_ignoredExtensions;
        TexturePtr m_folderTexture;

        std::vector<FileItem> m_FileItems;
        std::unordered_map<std::string, FileMetadata> m_FileMetadata;

        bool m_shouldUpdate = true;
    };
}