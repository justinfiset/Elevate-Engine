module;


#include <cstring>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <tuple>
#include <utility>

#include <imgui.h>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <glm/glm.hpp>

module Elevate.Editor.Panels.AssetBrowserPanel;

#ifdef EE_EDITOR_BUILD

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Paths;
import Elevate.Foundations.Files;
import Elevate.Foundations.Shell;
import Elevate.Foundations.Guid;
import Elevate.Foundations.Bytes;
// Core
import Elevate.Core.Application;
import Elevate.Core.Types.TypeRegistry;
import Elevate.Core.Assets.AssetRegistry;
import Elevate.Core.Assets.Asset;
import Elevate.Core.Assets.AssetMetadata;
// Renderer
import Elevate.Renderer.Texture;
// Serialization
import Elevate.Serialization.JsonSerializer;
// Input
import Elevate.Input;
// Editor
import Elevate.Editor.Resources;
import Elevate.Editor.Layer;
import Elevate.Editor.Widgets.ContextMenu;
import Elevate.Editor.Widgets.IconMenuItem;
import Elevate.Editor.Renderer;
import Elevate.Editor.Renderer.AssetPreviewSystem;

namespace fs = std::filesystem;

namespace Elevate::Editor
{
	static fs::path ResolveRenamePath(const fs::path& oldPath, const std::string& typedName)
	{
		fs::path newPath = oldPath.parent_path() / typedName;
		if (newPath.extension().empty() && !oldPath.extension().empty())
		{
			newPath += oldPath.extension();
		}
		return newPath;
	}

	static fs::path MakeUniquePath(const fs::path& desired)
	{
		if (!fs::exists(desired))
		{
			return desired;
		}

		const fs::path dir = desired.parent_path();
		const std::string stem = desired.stem().string();
		const std::string ext = desired.extension().string();

		for (int i = 1; ; i++)
		{
			fs::path candidate = dir / (stem + " (" + std::to_string(i) + ")" + ext);
			if (!fs::exists(candidate))
			{
				return candidate;
			}
		}
	}

	AssetBrowserPanel::AssetBrowserPanel()
	{
		s_instance = this;
		m_isRenaming = false;

		LoadExtensionsMeta();
		m_folderTexture = Texture::CreateFromFile(m_FileMetadata["DIRECTORY"].iconPath);

		m_CurrentPath = Paths::Content().lexically_normal();
		m_shouldUpdate = true;

		TextureMetadata texMeta = TextureMetadataBuilder()
			.Filter(TextureFilter::Linear, TextureFilter::Linear)
			.Mipmaps(false)
			.Usage(TextureType::Diffuse)
			.Build();

		m_newFolderTexture = Texture::CreateFromFile(Resources::Icons::Light::NewFolder, texMeta);
		m_deleteTexture = Texture::CreateFromFile(Resources::Icons::Light::Delete, texMeta);
		m_renameTexture = Texture::CreateFromFile(Resources::Icons::Light::Rename, texMeta);

		CoreLogger::Info("Editor Assets Browser Initiated.");
	}

	const FileItem* AssetBrowserPanel::GetItemFromIndex(uint32_t index) const
	{
		if (index == 0 || index > m_FileItems.size())
		{
			return nullptr;
		}
		return &m_FileItems[index - 1];
	}

	void AssetBrowserPanel::OnUpdate()
	{
		if (m_shouldUpdate)
		{
			UpdateRelatedPaths();
			LoadFileItemsList();
			m_selected.clear();
			m_lastSelected = 0;
			m_shouldUpdate = false;
		}
	}

	void AssetBrowserPanel::OnImGuiRender()
	{
		if (m_assetCreationList.empty())
		{
			BuildAssetNodeCache();
		}

		ImGui::Begin("Asset Browser");

		// Keyboard shortcuts (only when the panel is focused and no text field is active)
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !m_isRenaming && !ImGui::GetIO().WantTextInput)
		{
			if (ImGui::IsKeyPressed(ImGuiKey_F2, false))
			{
				StartRenameSelected();
			}

			if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
			{
				RequestDeleteSelected();
			}
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			// Right click on empty space : clear the selection so Rename/Delete are disabled
			if (!ImGui::IsAnyItemHovered())
			{
				m_selected.clear();
			}
			ImGui::OpenPopup("AssetContextMenu");
		}

		ImGui::BeginGroup();

		int id = 0;
		for (auto it = m_relatedPaths.rbegin(); it != m_relatedPaths.rend(); ++it)
		{
			ImGui::BeginDisabled(it->Path == m_CurrentPath);
			std::string buttonLabel = it->DisplayName + "##btn_" + std::to_string(id++);
			if (ImGui::Button(buttonLabel.c_str()))
			{
				m_CurrentPath = it->Path;
				m_shouldUpdate = true;
			}
			ImGui::EndDisabled();

			ImGui::SameLine();
			ImGui::Text("/");
			ImGui::SameLine();
		}
		ImGui::EndGroup();
		ImGui::Separator();

		ImVec2 buttonSize(72, 72);
		float spacing = ImGui::GetStyle().ItemSpacing.x * 2;
		float panelWidth = ImGui::GetWindowSize().x;
		int colNb = (int)std::floor(panelWidth / (buttonSize.x + spacing));

		colNb = std::max(1, colNb);

		int index = 0;

		if (m_CurrentPath != Paths::Content().lexically_normal() && m_CurrentPath.has_parent_path())
		{
			ImGui::PushID(index);
			ImGui::BeginGroup();
			ImGui::ImageButton("back", (ImTextureID)m_folderTexture->GetNativeHandle(), buttonSize);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
				m_shouldUpdate = true;
			}

			ImGui::TextWrapped("../");

			ImGui::EndGroup();
			ImGui::PopID();
			ImGui::SameLine();
			index++;
		}

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		float barThickness = 3.0f;
		uint32_t itemIndex = 0;
		for (FileItem item : m_FileItems)
		{
			itemIndex++;

			bool isSelected = std::find(m_selected.begin(), m_selected.end(), itemIndex) != m_selected.end();

			drawList->ChannelsSplit(2);
			drawList->ChannelsSetCurrent(1); // Draw the foreground

			ImGui::PushID(index);
			ImGui::BeginGroup();

			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			}

			ImTextureID icon = (ImTextureID)m_currentTextures[item.iconPath]->GetNativeHandle();
			ImVec2 uv0(0, 0), uv1(1, 1);

			// todo : inject directly instead of resolving every frame
			auto previewSystem = Application::Get().GetServices().Resolve<AssetPreviewSystem>();

			if (AssetRegistry::IsPathRegistered(item.path))
			{
				if (void* preview = previewSystem->GetPreview(AssetRegistry::GetPathGuid(item.path)))
				{
					icon = (ImTextureID)preview;
					uv0 = ImVec2(0, 1);
					uv1 = ImVec2(1, 0);
				}
			}

			if (ImGui::ImageButton("file_item", icon, buttonSize, uv0, uv1))
			{
			}
			const float itemWidth = ImGui::GetItemRectSize().x;

			if (isSelected)
			{
				ImGui::PopStyleColor(3);
			}

			ImVec2 barMin = ImGui::GetItemRectMin();
			ImVec2 barMax = ImGui::GetItemRectMax();
			barMin.y = barMax.y - barThickness;

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (item.type == Directory)
				{
					m_CurrentPath /= item.name;
					m_shouldUpdate = true;

					ImGui::EndGroup();
					ImGui::PopID();
					break;
				}
				else
				{
					Shell::OpenWithDefaultApp(item.path);
				}
			}

			ImGui::GetWindowDrawList()->AddRectFilled(
				barMin,
				barMax,
				IM_COL32(item.color.r, item.color.g, item.color.b, item.color.a),
				5.0f,
				ImDrawFlags_RoundCornersBottom);

			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + buttonSize.x);

			bool isThisItemRenaming = m_isRenaming && isSelected;

			// Escape cancels the rename
			if (isThisItemRenaming && ImGui::IsKeyPressed(ImGuiKey_Escape, false))
			{
				m_isRenaming = false;
				isThisItemRenaming = false;
			}

			bool renameInvalid = false;

			if (!isThisItemRenaming)
			{
				ImGui::TextWrapped("%s", item.name.c_str());
			}
			else
			{
				const fs::path target = ResolveRenamePath(item.path, m_renameBuffer);
				const bool nameTaken = target != fs::path(item.path) && fs::exists(target);
				const bool nameEmpty = m_renameBuffer[0] == '\0';
				renameInvalid = nameTaken || nameEmpty;

				if (renameInvalid)
				{
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.45f, 0.10f, 0.10f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.90f, 0.25f, 0.25f, 1.0f));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
				}

				ImGui::SetKeyboardFocusHere();
				std::string inputId = "##rename_" + std::to_string(itemIndex);

				ImGui::SetNextItemWidth(itemWidth);
				const bool submitted = ImGui::InputText(inputId.c_str(), m_renameBuffer, sizeof(m_renameBuffer),
					ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

				if (renameInvalid)
				{
					ImGui::SetTooltip("%s", nameTaken ? "A file with this name already exists." : "The name cannot be empty.");
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(2);
				}

				if (submitted && !renameInvalid)
				{
					RenameItem(item);
				}
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
			{
				if (isThisItemRenaming)
				{
					if (renameInvalid)
					{
						m_isRenaming = false;
					}
					else
					{
						RenameItem(item);
					}
				}
			}

			ImGui::PopTextWrapPos();

			ImGui::EndGroup();

			// Right click on an unselected item selects it first
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !isSelected)
			{
				m_selected.clear();
				m_selected.insert(itemIndex);
				m_lastSelected = itemIndex;
				SelectItemInEditor(item);
			}

			if (ImGui::IsItemClicked())
			{
				if (!ImGui::GetIO().KeyCtrl)
				{
					m_selected.clear();
					SelectItemInEditor(item);
				}

				if (ImGui::GetIO().KeyShift)
				{
					m_selected.clear();
					int start = std::min(itemIndex, m_lastSelected);
					int end = std::max(itemIndex, m_lastSelected);
					for (int i = start; i <= end; i++)
					{
						m_selected.insert(i);
					}
				}
				else
				{
					m_lastSelected = itemIndex;
				}

				m_selected.insert(itemIndex);
			}

			drawList->ChannelsSetCurrent(0);

			if (isSelected)
			{
				ImVec2 groupPadding = ImVec2(2.0f, 2.0f);
				ImVec2 groupMin = ImGui::GetItemRectMin();
				groupMin -= groupPadding;
				ImVec2 groupMax = ImGui::GetItemRectMax();
				groupMax += groupPadding;

				ImGui::GetWindowDrawList()->AddRect(
					groupMin,
					groupMax,
					IM_COL32(66, 150, 250, 255),
					4.0f,
					0,
					2.0f);

				ImGui::GetWindowDrawList()->AddRectFilled(
					groupMin,
					groupMax,
					IM_COL32(66, 150, 250, 128),
					4.0f,
					0);
			}

			drawList->ChannelsMerge();
			ImGui::PopID();

			if ((index + 1) % colNb != 0)
			{
				ImGui::SameLine();
			}

			index++;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
		{
			m_selected.clear();
		}

		if (Widgets::ContextMenu::Begin("AssetContextMenu"))
		{
			DrawContextMenu();
			Widgets::ContextMenu::End();
		}

		DrawDeleteConfirmation();

		ImGui::End();
	}

	void AssetBrowserPanel::StartRenameSelected()
	{
		if (m_selected.empty())
		{
			return;
		}

		const uint32_t index = *m_selected.begin();
		const FileItem* item = GetItemFromIndex(index);
		if (!item)
		{
			return;
		}

		// Only one item can be in rename mode
		m_selected.clear();
		m_selected.insert(index);
		m_lastSelected = index;

		std::strncpy(m_renameBuffer, item->name.c_str(), sizeof(m_renameBuffer) - 1);
		m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
		m_isRenaming = true;
	}

	void AssetBrowserPanel::RequestDeleteSelected()
	{
		m_pendingDelete.clear();

		for (uint32_t index : m_selected)
		{
			if (const FileItem* item = GetItemFromIndex(index))
			{
				m_pendingDelete.push_back(*item);
			}
		}

		if (!m_pendingDelete.empty())
		{
			m_openDeletePopup = true;
		}
	}

	void AssetBrowserPanel::DrawDeleteConfirmation()
	{
		if (m_openDeletePopup)
		{
			ImGui::OpenPopup("Delete items?");
			m_openDeletePopup = false;
		}

		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (!ImGui::BeginPopupModal("Delete items?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			return;
		}

		if (m_pendingDelete.size() == 1)
		{
			ImGui::Text("Delete '%s'?", m_pendingDelete[0].name.c_str());
		}
		else
		{
			ImGui::Text("Delete %zu items?", m_pendingDelete.size());

			const size_t shown = std::min<size_t>(m_pendingDelete.size(), 5);
			for (size_t i = 0; i < shown; i++)
			{
				ImGui::BulletText("%s", m_pendingDelete[i].name.c_str());
			}
			if (m_pendingDelete.size() > shown)
			{
				ImGui::TextDisabled("... and %zu more", m_pendingDelete.size() - shown);
			}
		}

		ImGui::TextDisabled("This cannot be undone.");
		ImGui::Separator();

		if (ImGui::Button("Delete", ImVec2(120, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			for (const FileItem& item : m_pendingDelete)
			{
				RemoveItem(item);
			}
			m_pendingDelete.clear();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)) || ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			m_pendingDelete.clear();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	void AssetBrowserPanel::SelectAsset(const Asset* asset)
	{
		auto* entry = AssetRegistry::GetEntry(asset->GetGuid());

		if (entry && s_instance)
		{
			s_instance->m_CurrentPath = entry->FilePath.parent_path();
			s_instance->UpdateRelatedPaths();

			s_instance->UpdateRelatedPaths();
			s_instance->LoadFileItemsList();
			s_instance->m_selected.clear();
			s_instance->m_lastSelected = 0;
			s_instance->m_shouldUpdate = false;

			uint32_t currentIdx = 0;
			for (const auto& item : s_instance->m_FileItems)
			{
				currentIdx++;
				if (fs::absolute(item.path) == fs::absolute(entry->FilePath))
				{
					s_instance->m_selected.insert(currentIdx);
					s_instance->m_lastSelected = currentIdx;
					break;
				}
			}
		}
		else
		{
			CoreLogger::Error("Could not select unregistered asset.");
		}
	}

	void AssetBrowserPanel::BuildAssetNodeCache()
	{
		for (const auto& [name, meta] : AssetRegistry::GetNameMetas())
		{
			if (meta.Flags & AssetFlags::CreateAssetMenu)
			{
				AssetCreationNode node;
				node.Extension = meta.Extension.c_str();
				node.Name = meta.TypeName.c_str();
				node.Factory = TypeRegistry::GetEntry(meta.TypeIndex).factory;
				m_assetCreationList.push_back(node);
			}
		}
	}

	void AssetBrowserPanel::DrawContextMenu()
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (Widgets::IconMenuItem("Create Folder", (ImTextureID)m_newFolderTexture->GetNativeHandle()))
			{
				try
				{
					fs::path path = MakeUniquePath(m_CurrentPath / "New_Folder");
					fs::create_directory(path);
					m_shouldUpdate = true;
				}
				catch (const fs::filesystem_error& e)
				{
					CoreLogger::Error("Failed to create folder: {}", e.what());
				}
			}

			ImGui::Separator();

			for (auto& type : m_assetCreationList)
			{
				if (ImGui::MenuItem(type.Name))
				{
					auto asset = type.Factory();

					if (!asset)
					{
						CoreLogger::Error("Failed to create new asset. Factory function returned null object.");
						continue;
					}

					JsonSerializer serializer;
					ByteBuffer bytes;
					serializer.Serialize(asset->GetProperties(), bytes);
					ByteUtils::ToString(bytes);

					const fs::path filePath = MakeUniquePath(fs::path(m_CurrentPath) / ("New_" + std::string(type.Name) + std::string(type.Extension)));
					std::ofstream outFile(filePath);

					if (outFile.is_open())
					{
						outFile << ByteUtils::ToString(bytes);
						outFile.close();
					}
					else
					{
						CoreLogger::Error("Failed to create new asset on disk at {}", filePath.string());
					}

					auto assetPtr = std::static_pointer_cast<Asset>(asset);
					AssetRegistry::RegisterAssetOnDisk(assetPtr, filePath.string());
					SelectAsset(assetPtr.get());

					// Use the file name (not the display name) so the rename field matches what is on disk
					const std::string fileName = filePath.filename().string();
					std::strncpy(m_renameBuffer, fileName.c_str(), sizeof(m_renameBuffer) - 1);
					m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
					m_isRenaming = true;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();

		const bool hasSelected = !m_selected.empty();

		if (Widgets::IconMenuItem("Rename", (ImTextureID)m_renameTexture->GetNativeHandle(), hasSelected))
		{
			StartRenameSelected();
		}

		if (Widgets::IconMenuItem("Delete", (ImTextureID)m_deleteTexture->GetNativeHandle(), hasSelected))
		{
			RequestDeleteSelected();
		}
	}

	void AssetBrowserPanel::UpdateRelatedPaths()
	{
		m_relatedPaths.clear();

		const auto currentNorm = fs::weakly_canonical(m_CurrentPath);
		const auto rootNorm = fs::weakly_canonical(Paths::Content().lexically_normal());

		std::string displayName = (currentNorm == rootNorm) ? "Game Content" : currentNorm.filename().string();
		m_relatedPaths.push_back({ m_CurrentPath, displayName });

		if (currentNorm != rootNorm)
		{
			AddParentPaths(m_CurrentPath);
		}
	}

	void AssetBrowserPanel::AddParentPaths(std::filesystem::path path)
	{
		if (!path.has_parent_path())
			return;

		std::filesystem::path parent = path.parent_path();
		const auto parentNorm = fs::weakly_canonical(parent);
		const auto rootNorm = fs::weakly_canonical(Paths::Content().lexically_normal());

		if (parentNorm == rootNorm)
		{
			m_relatedPaths.push_back({ parent, "Game Content" });
			return;
		}

		m_relatedPaths.push_back({ parent, parentNorm.filename().string() });

		if (parentNorm != rootNorm && parentNorm.string().find(rootNorm.string()) == 0)
		{
			AddParentPaths(parent);
		}
	}

	void AssetBrowserPanel::SelectItemInEditor(const FileItem& item)
	{
		if (!AssetRegistry::IsPathRegistered(item.path))
		{
			return;
		}

		Guid guid = AssetRegistry::GetPathGuid(item.path);
		auto entry = AssetRegistry::GetEntry(guid);
		if (!entry)
		{
			return;
		}

		if (!entry->isLoaded || !entry->Instance)
		{
			auto loadedAsset = AssetRegistry::GetAsset<Asset>(entry->AssetGuid);
			if (loadedAsset)
			{
				AssetRegistry::RegisterAsset(loadedAsset);
				entry = AssetRegistry::GetEntry(guid); // Update the entry
			}
		}

		if (entry && entry->Instance)
		{
			EEObjectPtr<EEObject> eeObject(entry->Instance);
			EditorLayer::Get().SelectObject(eeObject);
		}
	}

	void AssetBrowserPanel::RemoveItem(const FileItem& item)
	{
		fs::path fsPath = item.path;

		try
		{
			Guid guid = AssetRegistry::GetPathGuid(fsPath);

			fs::remove(fsPath);

			AssetRegistry::UnregisterAssetByPath(fsPath);

			if (guid.IsValid())
			{
				Application::Get().GetServices().Resolve<AssetPreviewSystem>()->Remove(guid);
			}

			m_shouldUpdate = true;
		}
		catch (const fs::filesystem_error& e)
		{
			CoreLogger::Error("Failed to remove file: {}", e.what());
		}
	}

	void AssetBrowserPanel::RenameItem(const FileItem& item)
	{
		std::string newName = std::string(m_renameBuffer);
		fs::path oldPath = item.path;
		fs::path newPath = ResolveRenamePath(oldPath, newName);

		// Nothing to do : empty or unchanged name
		if (newName.empty() || newPath == oldPath)
		{
			m_isRenaming = false;
			return;
		}

		// Do not silently overwrite another file
		if (fs::exists(newPath))
		{
			CoreLogger::Error("Cannot rename '{}' : '{}' already exists.", item.name, newPath.filename().string());
			m_isRenaming = false;
			return;
		}

		try
		{
			fs::rename(oldPath, newPath);
			AssetRegistry::UpdateAssetPath(oldPath, newPath);
			m_shouldUpdate = true;
			CoreLogger::Info("Renamed '{}' to '{}'", item.name, newPath.filename().string());
		}
		catch (const fs::filesystem_error& e)
		{
			CoreLogger::Error("Failed to rename file: {}", e.what());
		}

		m_isRenaming = false;
	}

	void AssetBrowserPanel::LoadFileItemsList()
	{
		m_FileItems.clear();
		m_currentTextures.clear();

		m_nextId = 0;

		if (!fs::exists(m_CurrentPath) || !fs::is_directory(m_CurrentPath))
		{
			CoreLogger::Error("AssetBrowser: m_CurrentPath is invalid. : {}", m_CurrentPath.string());
			return;
		}

		for (const auto& entry : fs::directory_iterator(m_CurrentPath))
		{
			FileMetadata meta;
			std::string ext = "";

			if (entry.is_directory())
			{
				if (fs::is_empty(entry.path()))
				{
					meta = m_FileMetadata["EMPTY_DIRECTORY"];
				}
				else
				{
					meta = m_FileMetadata["DIRECTORY"];
				}
			}
			else
			{
				ext = entry.path().extension().string();
				if (!ext.empty() && ext[0] == '.')
				{
					ext = ext.substr(1);
				}

				// if the current extension is ignored
				if (std::find(m_ignoredExtensions.begin(), m_ignoredExtensions.end(), ext) != m_ignoredExtensions.end())
				{
					continue;
				}
				else
				{
					if (m_FileMetadata.find(ext) != m_FileMetadata.end())
					{
						meta = m_FileMetadata[ext];
					}
					else
					{
						meta = m_FileMetadata["ANY"];
					}
				}
			}

			FileItem fileItem;
			if (meta.type == Image)
			{
				fileItem = FileItem(entry.path().string(), entry.path().filename().string(), ext, entry.path().string(), meta.type);
			}
			else
			{
				fileItem = FileItem(entry.path().string(), entry.path().filename().string(), ext, meta.iconPath, meta.type);
			}

			if (auto* assetMeta = AssetRegistry::GetMetaFromExtension("." + ext))
			{
				fileItem.color = assetMeta->AssetColor * 255.0f;
			}
			else
			{
				// Fallback if no asset has declared a color
				fileItem.color = meta.color;
			}

			fileItem.id = m_nextId++;

			m_currentTextures[fileItem.iconPath] = Texture::CreateFromFile(fileItem.iconPath);
			m_FileItems.push_back(fileItem);
		}

		std::sort(m_FileItems.begin(), m_FileItems.end(), [](const FileItem& a, const FileItem& b)
			{
				if (a.type == b.type)
				{
					return a.name < b.name;
				}
				else
				{
					return a.type < b.type;
				} });
	}

	void AssetBrowserPanel::LoadExtensionsMeta(std::string filepath)
	{
		auto resolvedPath = PathResolver::Resolve(filepath);
		FILE* fp = fopen(resolvedPath.string().c_str(), "r");
		if (!fp)
		{
			CoreLogger::Error("Cannot open JSON file : {}", resolvedPath.string());
			return;
		}

		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document doc;
		doc.ParseStream(is);
		fclose(fp);

		if (doc.HasParseError())
		{
			CoreLogger::Error("Erreur parsing JSON : %s", rapidjson::GetParseError_En(doc.GetParseError()));
			return;
		}

		if (doc.HasMember("ignore") && doc["ignore"].IsArray())
		{
			const rapidjson::Value& ignore = doc["ignore"];
			for (rapidjson::SizeType i = 0; i < ignore.Size(); i++)
			{
				const rapidjson::Value& filetype = ignore[i];
				if (filetype.IsString())
				{
					m_ignoredExtensions.push_back(filetype.GetString());
				}
			}
		}

		if (!doc.HasMember("assets") || !doc["assets"].IsArray())
		{
			CoreLogger::Error("Could not find valid assets key in JSON file.");
			return;
		}

		const rapidjson::Value& assets = doc["assets"];
		for (rapidjson::SizeType i = 0; i < assets.Size(); i++)
		{
			const rapidjson::Value& asset = assets[i];

			if (!asset.HasMember("extension") || !asset["extension"].IsString() ||
				!asset.HasMember("iconPath") || !asset["iconPath"].IsString() ||
				!asset.HasMember("type") || !asset["type"].IsString())
			{
				CoreLogger::Error("The asset {} is invalid (missing data or incorrect type)", i + 1);
				continue;
			}

			std::string extension = asset["extension"].GetString();
			std::string iconPath = asset["iconPath"].GetString();
			std::string typeStr = asset["type"].GetString();

			FileType type = FileMetadata::ParseFileType(typeStr);
			FileMetadata meta(type, iconPath);

			if (asset.HasMember("r") && asset.HasMember("g") && asset.HasMember("b") && asset.HasMember("a"))
			{
				int r = asset["r"].GetInt();
				int g = asset["g"].GetInt();
				int b = asset["b"].GetInt();
				int a = asset["a"].GetInt();
				meta.color = glm::vec4(r, g, b, a);
			}

			m_FileMetadata[extension] = meta;
		}
	}

	std::shared_ptr<Asset> AssetBrowserPanel::GetLoadedAsset(const FileItem& item)
	{
		if (!AssetRegistry::IsPathRegistered(item.path))
		{
			return nullptr;
		}

		Guid guid = AssetRegistry::GetPathGuid(item.path);
		auto entry = AssetRegistry::GetEntry(guid);

		if (!entry || !entry->isLoaded || !entry->Instance)
		{
			return nullptr;
		}

		return std::static_pointer_cast<Asset>(entry->Instance);
	}
}

#endif
