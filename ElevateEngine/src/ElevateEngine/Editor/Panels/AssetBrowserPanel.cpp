#include "eepch.h"
#include "AssetBrowserPanel.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <ElevateEngine/Core/Log.h>

Elevate::Editor::AssetBrowserPanel::AssetBrowserPanel(std::string filepath)
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
    fclose(fp);  // Fermer le fichier après parsing

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

void Elevate::Editor::AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

	ImGui::End();
}
