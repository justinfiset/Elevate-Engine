#include "eepch.h"
#include "AssetBrowserPanel.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <ElevateEngine/Core/Log.h>

#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Core/Files.h>

#include <ElevateEngine/Renderer/Texture/TextureManager.h>

#include <imgui.h>

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
        LoadFileItemsList();
        m_shouldUpdate = false;
    }
}

void Elevate::Editor::AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

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

    int id = 0;
    for (FileItem item : m_FileItems)
    {
        ImGui::PushID(index);
        ImGui::BeginGroup();

        if (ImGui::ImageButton("file_item", (ImTextureID) m_currentTextures[item.iconPath]->GetNativeHandle(), buttonSize)) {}

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (item.type == Directory) {
                m_CurrentPath += "/" + item.name;
                m_shouldUpdate = true;
            }
            else {
                Files::OpenWithDefaultApp(item.path);
            }
        }

        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + buttonSize.x);
        ImGui::TextWrapped("%s", item.name.c_str());
        ImGui::PopTextWrapPos();

        ImGui::EndGroup();
        ImGui::PopID();

        if ((index + 1) % colNb != 0)
        {
            ImGui::SameLine();
        }
        index++;
    }
	ImGui::End();
}

void Elevate::Editor::AssetBrowserPanel::LoadFileItemsList()
{
    m_FileItems.clear();
    m_currentTextures.clear();

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
    FILE* fp = fopen(filepath.c_str(), "r");
    if (!fp) {
        EE_CORE_ERROR("Cannot open JSON file : %s", filepath);
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
            EE_CORE_ERROR("The asset %s is invalid (missing data or incorrect type)", i + 1);
            continue;
        }

        std::string extension = asset["extension"].GetString();
        std::string iconPath = asset["iconPath"].GetString();
        std::string typeStr = asset["type"].GetString();

        FileType type = FileMetadata::ParseFileType(typeStr);
        FileMetadata meta(type, iconPath);
        m_FileMetadata[extension] = meta;   
    }
}
