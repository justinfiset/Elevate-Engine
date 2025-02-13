#include "eepch.h"
#include "AssetBrowserPanel.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <ElevateEngine/Core/Log.h>
#include <filesystem>

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace fs = std::filesystem;

Elevate::Editor::AssetBrowserPanel::AssetBrowserPanel()
{
    LoadExtensionsMeta();
    LoadFileItemsList();
    EE_CORE_INFO("Editor Assets Browser Initiated.");
}

void Elevate::Editor::AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

    ImVec2 buttonSize(72, 72);
    float spacing = ImGui::GetStyle().ItemSpacing.x * 2;
    float panelWidth = ImGui::GetWindowSize().x;
    int colNb = std::floor(panelWidth / (buttonSize.x + spacing));
    colNb = std::max(1, colNb);

    int index = 0;
    for (FileItem item : m_FileItems)
    {
        TexturePtr texture = Texture::Create(item.metadata.iconPath); 
        ImGui::BeginGroup();
        if (ImGui::ImageButton("Mon Boutton", (void*)(intptr_t)texture->GetID(), buttonSize)) {
            printf("Image Button Clicked!\n");
        }
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + buttonSize.x);
        ImGui::TextWrapped(item.name.c_str());
        ImGui::PopTextWrapPos();

        ImGui::EndGroup();

        if ((index + 1) % colNb != 0)
        {
            ImGui::SameLine();
        }
        index++;
    }
	ImGui::End();
}

void Elevate::Editor::AssetBrowserPanel::LoadFileItemsList(std::string path)
{
    m_FileItems.clear();
    for (const auto& entry : fs::directory_iterator(path)) {
        FileMetadata meta;
        std::string ext = "";

        if (entry.is_directory()) {
            meta = m_FileMetadata["DIRECTORY"];
        }
        else {
            ext = entry.path().extension().string();
            if (m_FileMetadata.find(ext) != m_FileMetadata.end()) {
                meta = m_FileMetadata[ext];
            }
            else {
                meta = m_FileMetadata["ANY"];
            }
        }
        FileItem fileItem(entry.path().string(), entry.path().filename().string(), ext, meta);
        m_FileItems.push_back(fileItem);
    }
}

void Elevate::Editor::AssetBrowserPanel::LoadExtensionsMeta(std::string filepath)
{
    FILE* fp = fopen(filepath.c_str(), "r");
    if (!fp) {
        EE_CORE_ERROR("Impossible d'ouvrir le fichier JSON: {0}", filepath);
        return;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) {
        EE_CORE_ERROR("Erreur parsing JSON : {0}", rapidjson::GetParseError_En(doc.GetParseError()));
        return;
    }

    if (!doc.HasMember("assets") || !doc["assets"].IsArray()) {
        EE_CORE_ERROR("Clé 'assets' non trouvée ou invalide dans le fichier JSON");
        return;
    }

    const rapidjson::Value& assets = doc["assets"];
    for (rapidjson::SizeType i = 0; i < assets.Size(); i++) {
        const rapidjson::Value& asset = assets[i];

        if (!asset.HasMember("extension") || !asset["extension"].IsString() ||
            !asset.HasMember("iconPath") || !asset["iconPath"].IsString() ||
            !asset.HasMember("type") || !asset["type"].IsString()) {
            EE_CORE_ERROR("L'asset {0} est invalide (données manquantes ou type incorrect)", i + 1);
            continue;
        }

        std::string extension = asset["extension"].GetString();
        std::string iconPath = asset["iconPath"].GetString();
        std::string typeStr = asset["type"].GetString();

        FileType type = FileMetadata::ParseFileType(typeStr);
        FileMetadata meta(type, iconPath);
        m_FileMetadata[extension] = meta;

        // todo add debug level
        //EE_CORE_INFO("Asset {0} :", i + 1);
        //EE_CORE_TRACE("  Extension: {0}", extension);
        //EE_CORE_TRACE("  Icon Path: {0}", iconPath);
        //EE_CORE_TRACE("  Type: {0}", (type == File ? "FILE" : "DIRECTORY"));
    }
}
