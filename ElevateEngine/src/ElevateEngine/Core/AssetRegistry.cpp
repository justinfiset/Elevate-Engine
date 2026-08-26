#include "AssetRegistry.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <ElevateEngine/Core/Core.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/EEObject.h>

namespace fs = std::filesystem;

namespace Elevate
{
    std::unordered_map<Guid, AssetEntry> AssetRegistry::s_indexedAssets;

    void AssetRegistry::Init()
    {
        Get()._Init();
    }

    void AssetRegistry::RefreshFromDisk()
    {
        fs::path rootPath = EE_CONTENT_ROOT;
        if (!fs::exists(rootPath) || !fs::is_directory(rootPath))
        {
            EE_CORE_ERROR("Content root dir could not be resolved by Asset Registry.");
        }

        auto options = fs::directory_options::skip_permission_denied;

        std::error_code ec;
        for (const auto& entry : fs::recursive_directory_iterator(rootPath, options, ec))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            std::string extension = entry.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            auto it = m_extensionMeta.find(extension);
            if (it != m_extensionMeta.end())
            {
                const AssetMetaData& typeMeta = it->second;
                Guid guid = ExtractGuidFromFile(entry.path());
                s_indexedAssets[guid] = AssetEntry{
                    .Guid = guid,
                    .FilePath = entry.path(),
                    .TypeIndex = typeMeta.TypeIndex,
                    .isOnDisk = true,
                    .isLoaded = false,
                    .Instance = nullptr
                };
            }
        }
    }

    std::vector<Guid> AssetRegistry::GetAssetsOfType(std::type_index typeIndex)
    {
        std::vector<Guid> guids;
        for (const auto& [guid, entry] : s_indexedAssets)
        {
            if (entry.TypeIndex == typeIndex)
            {
                guids.push_back(guid);
            }
        }
        return guids;
    }

    void AssetRegistry::_Init()
    {
        EE_CORE_INFO("Initializing Asset Registry...");
        RefreshFromDisk();
    }

    Guid AssetRegistry::ExtractGuidFromFile(std::filesystem::path path)
    {
        FILE* fp = fopen(path.string().c_str(), "r");
        if (!fp)
        {
            EE_CORE_ERROR("Cannot open Asset JSON file : {}", path.string());
            return Guid{};
        }

        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        rapidjson::Document doc;
        doc.ParseStream(is);
        fclose(fp);

        if (doc.HasParseError())
        {
            EE_CORE_ERROR("Erreur parsing JSON : %s", rapidjson::GetParseError_En(doc.GetParseError()));
            return Guid{};
        }

        if (doc.HasMember(EEObject::GuidFieldName) && doc[EEObject::GuidFieldName].IsString())
        {
            std::string res = doc[EEObject::GuidFieldName].GetString();
            EE_CORE_WARN("Found asset with GUID : {}", res);
            return Guid::FromString(res);
        }
        else
        {
            EE_CORE_ERROR("Could not find valid guid key in JSON file.");
        }

        return Guid{};
    }

    const AssetMetaData* AssetRegistry::GetMetaFromExtension(const std::string& extension)
    {
        auto it = Get().m_extensionMeta.find(extension);
        return (it != Get().m_extensionMeta.end()) ? &it->second : nullptr;
    }

    const AssetMetaData* AssetRegistry::GetMetaFromTypeName(const std::string & typeName)
    {
        auto it = Get().m_nameMeta.find(typeName);
        return (it != Get().m_nameMeta.end()) ? &it->second : nullptr;
    }

    void AssetRegistry::RegisterAssetType(AssetMetaData trait)
    {
        auto& instance = Get();
        instance.m_nameMeta[trait.TypeName] = trait;

        // Lower the ext to standardize
        std::string ext = trait.Extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        instance.m_extensionMeta[ext] = trait;
    }

    void AssetRegistry::RegisterAsset(const Asset* asset)
    {
        if (!asset)
        {
            EE_CORE_ERROR("(AssetRegistry::RegisterAsset) : Tried to register a nullptr Asset in the Asset Registry.");
        }

        Guid assetGuid = asset->GetGuid();
        if (s_indexedAssets.contains(assetGuid))
        {
            AssetEntry& entry = s_indexedAssets.at(assetGuid);
            entry.Instance.reset(asset);
            entry.isLoaded = true;
        }
        else
        {
            AssetEntry entry{
                .Guid = assetGuid,
                .FilePath = "[Runtime Asset]",
                .TypeIndex = asset->GetTypeIndex(),
                .isOnDisk = false,
                .isLoaded = true,
            };
            entry.Instance.reset(asset);
            s_indexedAssets[assetGuid] = entry;
        }
    }
}
