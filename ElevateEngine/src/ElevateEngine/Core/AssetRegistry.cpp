#include "AssetRegistry.h"

namespace Elevate
{
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
        instance.m_extensionMeta[trait.Extension] = trait;
    }
}
