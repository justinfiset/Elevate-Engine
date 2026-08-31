#include "AssetBrowserPanel.h"

#include <algorithm>
#include <cmath>
#include <fstream>

#include <imgui.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Core/PathResolver.h>
#include <ElevateEngine/Core/TypeRegistry.h>
#include <ElevateEngine/Core/AssetRegistry.h>
#include <ElevateEngine/Core/Files.h>

#include <ElevateEngine/Inputs/Input.h>

#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>

#include <ElevateEngine/Serialization/JsonSerializer.h>

#include <ElevateEngine/Editor/EditorLayer.h>

namespace fs = std::filesystem;

const auto rootPath = fs::path(EE_CONTENT_ROOT).lexically_normal();

Elevate::Editor::AssetBrowserPanel::AssetBrowserPanel()
{
	s_instance = this;

	LoadExtensionsMeta();
	m_folderTexture = Texture::CreateFromFile(m_FileMetadata["DIRECTORY"].iconPath);

	m_CurrentPath = rootPath;
	m_shouldUpdate = true;

	EE_CORE_INFO("Editor Assets Browser Initiated.");
}

void Elevate::Editor::AssetBrowserPanel::OnUpdate()
{
	if (m_shouldUpdate) {
		UpdateRelatedPaths();
		LoadFileItemsList();
		m_selected.clear();
		m_lastSelected = 0;
		m_shouldUpdate = false;
	}
}

void Elevate::Editor::AssetBrowserPanel::OnImGuiRender()
{
	if (m_assetCreationList.empty())
	{
		BuildAssetNodeCache();
	}

	ImGui::Begin("Asset Browser");

	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
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
	int colNb = (int) std::floor(panelWidth / (buttonSize.x + spacing));
	colNb = std::max(1, colNb);

	int index = 0;

	if (m_CurrentPath != rootPath && m_CurrentPath.has_parent_path())
	{
		ImGui::PushID(index);
		ImGui::BeginGroup();
		ImGui::ImageButton("back", (ImTextureID) m_folderTexture->GetNativeHandle(), buttonSize);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
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
		
		if (ImGui::ImageButton("file_item", (ImTextureID) m_currentTextures[item.iconPath]->GetNativeHandle(), buttonSize)) {}

		if (isSelected)
		{
			ImGui::PopStyleColor(3);
		}

		ImVec2 barMin = ImGui::GetItemRectMin();
		ImVec2 barMax = ImGui::GetItemRectMax();
		barMin.y = barMax.y - barThickness;

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			if (item.type == Directory) {
				m_CurrentPath /= item.name;
				m_shouldUpdate = true;

				ImGui::EndGroup();
				ImGui::PopID();
				break;
			}
			else {
				Files::OpenWithDefaultApp(item.path);
			}
		}

		ImGui::GetWindowDrawList()->AddRectFilled(
			barMin,
			barMax,
			IM_COL32(item.color.r, item.color.g, item.color.b, item.color.a),
			5.0f,
			ImDrawFlags_RoundCornersBottom
		);

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + buttonSize.x);
		ImGui::TextWrapped("%s", item.name.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup();

		if (ImGui::IsItemClicked()) {
			if (!ImGui::GetIO().KeyCtrl)
			{
				m_selected.clear();
				
				if (AssetRegistry::IsPathRegistered(item.path))
				{
					Guid guid = AssetRegistry::GetPathGuid(item.path);
					auto entry = AssetRegistry::GetEntry(guid);

					if (entry)
					{
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
				}
			}

			if (ImGui::GetIO().KeyShift)
			{
				m_selected.clear();
				int start = std::min(itemIndex, m_lastSelected);
				int end = std::max(itemIndex, m_lastSelected);
				for (int i = start; i <= end; i++) {
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
				2.0f
			);

			ImGui::GetWindowDrawList()->AddRectFilled(
				groupMin,
				groupMax,
				IM_COL32(66, 150, 250, 128),
				4.0f,
				0
			);
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

	if (ImGui::BeginPopup("AssetContextMenu"))
	{
		DrawContextMenu();
		ImGui::EndPopup();
	}

	ImGui::End();
}

void Elevate::Editor::AssetBrowserPanel::SelectAsset(const Asset* asset)
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
		EE_CORE_ERROR("Could not select unregistered asset.");
	}
}

void Elevate::Editor::AssetBrowserPanel::BuildAssetNodeCache()
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

void Elevate::Editor::AssetBrowserPanel::DrawContextMenu()
{
	if (ImGui::BeginMenu("Create"))
	{
		for (auto& type : m_assetCreationList)
		{
			if (ImGui::MenuItem(type.Name))
			{
				auto asset = type.Factory();

				if (!asset)
				{
					EE_ERROR("Failed to create new asset. Factory function returned null object.");
					return;
				}

				JsonSerializer serializer;
				ByteBuffer bytes;
				serializer.Serialize(asset->GetProperties(), bytes);
				ByteUtils::ToString(bytes);

				std::string filePath = m_CurrentPath.string() + "/New_" + std::string(type.Name) + std::string(type.Extension);
				std::ofstream outFile(filePath);

				if (outFile.is_open())
				{
					outFile << Elevate::ByteUtils::ToString(bytes);
					outFile.close();
				}
				else
				{
					EE_ERROR("Failed to create new asset on disk at {}", filePath);
				}

				auto assetPtr = std::static_pointer_cast<Asset>(asset);
				AssetRegistry::RegisterAssetOnDisk(assetPtr, filePath);
				SelectAsset(assetPtr.get());
			}
		}
		ImGui::EndMenu();
	}
}

void Elevate::Editor::AssetBrowserPanel::UpdateRelatedPaths()
{
	m_relatedPaths.clear();

	const auto currentNorm = fs::weakly_canonical(m_CurrentPath);
	const auto rootNorm = fs::weakly_canonical(rootPath);

	std::string displayName = (currentNorm == rootNorm) ? "Game Content" : currentNorm.filename().string();
	m_relatedPaths.push_back({ m_CurrentPath, displayName });

	if (currentNorm != rootNorm)
	{
		AddParentPaths(m_CurrentPath);
	}
}

void Elevate::Editor::AssetBrowserPanel::AddParentPaths(std::filesystem::path path)
{
	if (!path.has_parent_path())
		return;

	std::filesystem::path parent = path.parent_path();
	const auto parentNorm = fs::weakly_canonical(parent);
	const auto rootNorm = fs::weakly_canonical(rootPath);

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

void Elevate::Editor::AssetBrowserPanel::LoadFileItemsList()
{
	m_FileItems.clear();
	m_currentTextures.clear();

	m_nextId = 0;

	if (!fs::exists(m_CurrentPath) || !fs::is_directory(m_CurrentPath))
	{
		EE_CORE_ERROR("AssetBrowser: m_CurrentPath is invalid. : {}", m_CurrentPath.string());
		return;
	}

	for (const auto& entry : fs::directory_iterator(m_CurrentPath)) {
		FileMetadata meta;
		std::string ext = "";

		if (entry.is_directory()) {
			if (fs::is_empty(entry.path())) {
				meta = m_FileMetadata["EMPTY_DIRECTORY"];
			}
			else {
				meta = m_FileMetadata["DIRECTORY"];
			}
		}
		else {
			ext = entry.path().extension().string();
			if (!ext.empty() && ext[0] == '.') {
				ext = ext.substr(1);
			}

			// if the current extension is ignored
			if (std::find(m_ignoredExtensions.begin(), m_ignoredExtensions.end(), ext) != m_ignoredExtensions.end())
			{
				continue;
			}
			else
			{
				if (m_FileMetadata.find(ext) != m_FileMetadata.end()) {
					meta = m_FileMetadata[ext];
				}
				else {
					meta = m_FileMetadata["ANY"];
				}
			}
		}
		FileItem fileItem;
		if (meta.type == Image) {
			fileItem = FileItem(entry.path().string(), entry.path().filename().string(), ext, entry.path().string(), meta.type);
		}
		else {
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

	std::sort(m_FileItems.begin(), m_FileItems.end(), [](const FileItem& a, const FileItem& b) {
		if (a.type == b.type)
		{
			return a.name < b.name;
		}
		else
		{
			return a.type < b.type;
		}
	});
}

void Elevate::Editor::AssetBrowserPanel::LoadExtensionsMeta(std::string filepath)
{
	std::string resolvedPath = PathResolver::Resolve(filepath);
	FILE* fp = fopen(resolvedPath.c_str(), "r");
	if (!fp)
	{
		EE_CORE_ERROR("Cannot open JSON file : {}", resolvedPath);
		return;
	}

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document doc;
	doc.ParseStream(is);
	fclose(fp);

	if (doc.HasParseError()) {
		EE_CORE_ERROR("Erreur parsing JSON : %s", rapidjson::GetParseError_En(doc.GetParseError()));
		return;
	}

	if (doc.HasMember("ignore") && doc["ignore"].IsArray()) {
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

	if (!doc.HasMember("assets") || !doc["assets"].IsArray()) {
		EE_CORE_ERROR("Could not find valid assets key in JSON file.");
		return;
	}

	const rapidjson::Value& assets = doc["assets"];
	for (rapidjson::SizeType i = 0; i < assets.Size(); i++) {
		const rapidjson::Value& asset = assets[i];

		if (!asset.HasMember("extension") || !asset["extension"].IsString() ||
			!asset.HasMember("iconPath") || !asset["iconPath"].IsString() ||
			!asset.HasMember("type") || !asset["type"].IsString()) {
			EE_CORE_ERROR("The asset {} is invalid (missing data or incorrect type)", i + 1);
			continue;
		}

		std::string extension = asset["extension"].GetString();
		std::string iconPath = asset["iconPath"].GetString();
		std::string typeStr = asset["type"].GetString();

		FileType type = FileMetadata::ParseFileType(typeStr);
		FileMetadata meta(type, iconPath);

		if (asset.HasMember("r") && asset.HasMember("g") && asset.HasMember("b") && asset.HasMember("a")) {
			int r = asset["r"].GetInt();
			int g = asset["g"].GetInt();
			int b = asset["b"].GetInt();
			int a = asset["a"].GetInt();
			meta.color = glm::vec4(r, g, b, a);
		}

		m_FileMetadata[extension] = meta;   
	}
}
