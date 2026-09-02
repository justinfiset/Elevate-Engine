#include "AssetRegistry.h"

#include <cstdio>
#include <system_error>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <ElevateEngine/Core/Core.h>
#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Core/AssetMetaData.h>
#include <ElevateEngine/Core/Byte.h>
#include <ElevateEngine/Core/Guid.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/TypeRegistry.h>
#include <ElevateEngine/Serialization/JsonSerializer.h>
#include <ElevateEngine/Files/FileUtility.h>

namespace fs = std::filesystem;

namespace Elevate
{
    std::unordered_map<Guid, AssetEntry> AssetRegistry::s_indexedAssets;
    std::unordered_map<std::filesystem::path, Guid> AssetRegistry::s_pathRegistry;

    std::string FormatAssetNameForUI(const std::filesystem::path& filePath)
    {
        std::string rawName = filePath.stem().string();

        std::string result;
        result.reserve(rawName.size());

        bool capitalizeNext = true;
        for (char c : rawName)
        {
            if (c == '_' || c == '-')
            {
                result += ' ';
                capitalizeNext = true;
            }
            else
            {
                if (capitalizeNext && std::isalpha(static_cast<unsigned char>(c)))
                {
                    result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                    capitalizeNext = false;
                }
                else
                {
                    result += c;
                }
            }
        }

        return result;
    }

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
                    .AssetGuid = guid,
                    .AssetName = FormatAssetNameForUI(entry),
                    .FilePath = entry.path(),
                    .TypeIndex = typeMeta.TypeIndex,
                    .isOnDisk = true,
                    .isLoaded = false,
                    .MetaData = &typeMeta,
                    .Instance = nullptr
                };
                
                fs::path normalizedPath = fs::absolute(entry.path()).lexically_normal();
                s_pathRegistry[normalizedPath] = guid;
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

    const AssetEntry* AssetRegistry::GetEntry(const Guid& guid)
    {
        auto it = s_indexedAssets.find(guid);
        if (it != s_indexedAssets.end())
        {
            return &it->second;
        }
        return nullptr;
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
            EE_CORE_TRACE("Found asset with GUID : {}", res);
            return Guid::FromString(res);
        }
        else
        {
            EE_CORE_ERROR("Could not find valid guid key in JSON file.");
        }

        return Guid{};
    }

    std::shared_ptr<Asset> AssetRegistry::LoadAssetFromDisk(const AssetEntry& entry)
    {
        const AssetMetaData* meta = entry.MetaData;
        if (!meta)
        {
            EE_CORE_ERROR("(AssetRegistry::LoadAssetFromDis) : Asset Meta Data was not provided to create asset from disk. Please add the EE_Asset tag to your asset.");
            return nullptr;
        }

        auto& typeEntry = TypeRegistry::GetEntry(entry.TypeIndex);
        if (!typeEntry.factory)
        {
            EE_CORE_ERROR("(AssetRegistry::LoadAssetFromDisk) : No factory registered for type {}. Object cannot be instantiated.", typeEntry.name);
            return nullptr;
        }

        std::shared_ptr<EEObject> rawObject = typeEntry.factory();
        std::shared_ptr<Asset> asset = std::dynamic_pointer_cast<Asset>(rawObject);
        if (!asset)
        {
            EE_CORE_ERROR("(AssetRegistry::LoadAssetFromDisk) : Type {} derives from EEObject but NOT from Asset.", typeEntry.name);
            return nullptr;
        }

        JsonSerializer serializer;
        PropertySet props;

        std::string diskContent = File::GetFileContent(entry.FilePath.string());
        serializer.Deserialize(ByteUtils::FromString(diskContent), props);
        asset->SetFromProperties(props);

        asset->SetGuid(entry.AssetGuid);
        asset->OnLoad();

        return asset;
    }

    const AssetMetaData* AssetRegistry::GetMetaFromExtension(const std::string& extension)
    {
        auto it = Get().m_extensionMeta.find(extension);
        return (it != Get().m_extensionMeta.end()) ? &it->second : nullptr;
    }

    const AssetMetaData* AssetRegistry::GetMetaFromTypeIndex(std::type_index typeIndex)
    {
        auto it = Get().m_typeMeta.find(typeIndex);
        return (it != Get().m_typeMeta.end()) ? &it->second : nullptr;
    }

    bool AssetRegistry::IsPathRegistered(std::filesystem::path path)
    {
        fs::path normalizedPath = fs::absolute(path).lexically_normal();
        return s_pathRegistry.find(normalizedPath) != s_pathRegistry.end();
    }

    Guid AssetRegistry::GetPathGuid(std::filesystem::path path)
    {
        fs::path normalizedPath = fs::absolute(path).lexically_normal();
    auto it = s_pathRegistry.find(normalizedPath);
    return (it != s_pathRegistry.end()) ? it->second : Guid{};
    }

    const std::unordered_map<std::string, AssetMetaData>& AssetRegistry::GetNameMetas()
    {
        return Get().m_nameMeta;
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

        instance.m_typeMeta[trait.TypeIndex] = trait;
    }

    void AssetRegistry::RegisterAsset(const std::shared_ptr<Asset>& asset)
    {
        if (!asset)
        {
            EE_CORE_ERROR("(AssetRegistry::RegisterAsset) : Tried to register a nullptr Asset in the Asset Registry.");
            return;
        }

        const AssetMetaData* metaData = GetMetaFromTypeIndex(asset->GetTypeIndex());
        if (!metaData)
        {
            EE_CORE_WARN("(AssetRegistry::RegisterAsset) : No metadata found for type {}", asset->GetTypeIndex().name());
            return;
        }

        Guid assetGuid = asset->GetGuid();
        if (s_indexedAssets.contains(assetGuid))
        {
            AssetEntry& entry = s_indexedAssets.at(assetGuid);
            entry.Instance = asset;
            entry.isLoaded = true;
            entry.MetaData = metaData;
        }
        else
        {
            AssetEntry entry{
                .AssetGuid = assetGuid,
                .AssetName = asset->GetName(),
                .FilePath = "[Runtime Asset]",
                .TypeIndex = asset->GetTypeIndex(),
                .isOnDisk = false,
                .isLoaded = true,
                .MetaData = metaData
            };
            entry.AssetName = asset->GetName();
            entry.Instance = asset;
            s_indexedAssets[assetGuid] = entry;
        }
    }

    void AssetRegistry::RegisterAssetOnDisk(const std::shared_ptr<Asset>& asset, const std::filesystem::path& filePath)
    {
        if (!asset)
        {
            EE_CORE_ERROR("(AssetRegistry::RegisterAssetOnDisk) : Tried to register a nullptr Asset.");
            return;
        }

        const AssetMetaData* metaData = GetMetaFromTypeIndex(asset->GetTypeIndex());
        Guid assetGuid = asset->GetGuid();

        AssetEntry entry{
            .AssetGuid = assetGuid,
            .AssetName = FormatAssetNameForUI(filePath),
            .FilePath = filePath,
            .TypeIndex = asset->GetTypeIndex(),
            .isOnDisk = true,
            .isLoaded = true,
            .MetaData = metaData,
            .Instance = asset
        };

        s_indexedAssets[assetGuid] = entry;
        
        fs::path normalizedPath = fs::absolute(filePath).lexically_normal();
        s_pathRegistry[normalizedPath] = assetGuid;
    }
}
