#include "AssetBrowserPanel.h"

#include <algorithm>
#include <cmath>

#include <imgui.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/PathResolver.h>
#include <ElevateEngine/Core/Files.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>

namespace fs = std::filesystem;

Elevate::Editor::AssetBrowserPanel::AssetBrowserPanel()
{
	LoadExtensionsMeta();
	m_folderTexture = Texture::CreateFromFile(m_FileMetadata["DIRECTORY"].iconPath);
	EE_CORE_INFO("Editor Assets Browser Initiated.");
}

void Elevate::Editor::AssetBrowserPanel::OnUpdate()
{
	if (m_shouldUpdate) {
		UpdateRelatedPaths();
		LoadFileItemsList();
		m_selected.clear();
		m_shouldUpdate = false;
	}
}

void Elevate::Editor::AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

	ImGui::BeginGroup();
	for (auto it = m_relatedPaths.rbegin(); it != m_relatedPaths.rend(); ++it)
	{
		ImGui::BeginDisabled(it->Path == m_CurrentPath);
		if (ImGui::Button(it->DisplayName.c_str()))
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

	if (m_CurrentPath != ".") {
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

	int id = 0;
	float barThickness = 3.0f;
	for (FileItem item : m_FileItems)
	{
		bool isSelected = std::find(m_selected.begin(), m_selected.end(), item.id) != m_selected.end();

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
				m_CurrentPath += "/" + item.name;
				m_shouldUpdate = true;
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
			m_selected.clear();
			m_selected.push_back(item.id);
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
	ImGui::End();
}

void Elevate::Editor::AssetBrowserPanel::UpdateRelatedPaths()
{
	m_relatedPaths.clear();

	std::string displayName = m_CurrentPath == "." ? "Game Content" : m_CurrentPath.filename().string();
	m_relatedPaths.push_back({ m_CurrentPath, displayName });
	AddParentPaths(m_CurrentPath);
}

void Elevate::Editor::AssetBrowserPanel::AddParentPaths(std::filesystem::path path)
{
	if (path.has_parent_path())
	{
		std::filesystem::path parent = path.parent_path();
		std::string displayName = parent == "." ? "Game Content" : parent.filename().string();
		m_relatedPaths.push_back({ parent, displayName });
		AddParentPaths(parent);
	}
}

void Elevate::Editor::AssetBrowserPanel::LoadFileItemsList()
{
	m_FileItems.clear();
	m_currentTextures.clear();

	m_nextId = 0;

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
		fileItem.color = meta.color;
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
	if (!fp) {
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
			int b = asset["g"].GetInt();
			int a = asset["g"].GetInt();
			meta.color = glm::vec4(r, g, b, a);
		}

		m_FileMetadata[extension] = meta;   
	}
}
