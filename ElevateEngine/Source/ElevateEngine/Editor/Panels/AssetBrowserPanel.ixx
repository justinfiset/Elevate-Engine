module;

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <memory>
#include <set>

#include <glm/vec4.hpp>

export module Elevate.Editor.Panels.AssetBrowserPanel;

#ifdef EE_EDITOR_BUILD

// Core
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Assets.Asset;
// Renderer
import Elevate.Renderer.Texture;
import Elevate.Renderer.Framebuffers;
// Editor
import Elevate.Editor.Widgets.Widget;

export namespace Elevate::Editor
{
	enum FileType
	{
		Directory = 0,
		File = 1,
		Internal = 2,
		Image = 3
	};

	struct FileMetadata
	{
		FileType type;
		std::string iconPath;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 0.0f };

		FileMetadata() = default;
		FileMetadata(FileType type, std::string iconPath)
		{
			this->type = type;
			this->iconPath = iconPath;
		}

		static FileType ParseFileType(std::string typeStr)
		{
			if (typeStr == "DIRECTORY")
			{
				return FileType::Directory;
			}
			else if (typeStr == "FILE")
			{
				return FileType::File;
			}
			else if (typeStr == "IMAGE")
			{
				return FileType::Image;
			}
			else
			{
				return FileType::Internal;
			}
		}
	};

	struct FileItem
	{
		uint32_t id = 0;
		std::string name;
		std::string path;
		std::string extension;
		std::string iconPath;
		glm::vec4 color;
		FileType type = File;

		FileItem() = default;
		FileItem(std::string filePath, std::string fileName, std::string fileExtension, std::string icon, FileType type) : path(filePath), name(fileName), extension(fileExtension), iconPath(icon), type(type)
		{
		}
	};

	struct BrowserPath
	{
		std::filesystem::path Path;
		std::string DisplayName;
	};

	class AssetBrowserPanel : public Widget
	{
	public:
		AssetBrowserPanel();

		void OnUpdate() override;
		void OnImGuiRender() override;

		static void SelectAsset(const Asset* asset);

	private:
		inline static AssetBrowserPanel* s_instance;

		struct AssetCreationNode
		{
			const char* Name;
			const char* Extension;
			std::function<std::shared_ptr<EEObject>()> Factory = nullptr;
		};
		void BuildAssetNodeCache();
		std::vector<AssetCreationNode> m_assetCreationList;

		void DrawContextMenu();

		// Update the paths at the top of the browser
		void UpdateRelatedPaths();
		void AddParentPaths(std::filesystem::path path);

		void SelectItemInEditor(const FileItem& item);

		void RemoveItem(const FileItem& item);
		void RenameItem(const FileItem& item);

		void LoadFileItemsList();
		void LoadExtensionsMeta(std::string filepath = "editor://Config/file_browser.json");

		std::shared_ptr<Asset> GetLoadedAsset(const FileItem& item);

		const FileItem* GetItemFromIndex(uint32_t index) const;
		void StartRenameSelected();
		void RequestDeleteSelected();
		void DrawDeleteConfirmation();

		std::filesystem::path m_CurrentPath;
		std::vector<BrowserPath> m_relatedPaths;

		//      < Path , Texture >
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_currentTextures;
		std::vector<std::string> m_ignoredExtensions;
		std::shared_ptr<Texture> m_folderTexture;

		std::vector<FileItem> m_FileItems;
		std::unordered_map<std::string, FileMetadata> m_FileMetadata;

		bool m_shouldUpdate;

		// Object rename
		bool m_isRenaming;
		char m_renameBuffer[256] = "";

		// Deletion
		std::vector<FileItem> m_pendingDelete;
		bool m_openDeletePopup = false;

		std::set<uint32_t> m_selected;
		uint32_t m_lastSelected = 0;
		uint32_t m_nextId = 0;

		std::shared_ptr<Texture> m_newFolderTexture;
		std::shared_ptr<Texture> m_renameTexture;
		std::shared_ptr<Texture> m_deleteTexture;

		bool m_previewDirty = true;
		std::unique_ptr<Framebuffer> m_preview;
	};
}

#endif