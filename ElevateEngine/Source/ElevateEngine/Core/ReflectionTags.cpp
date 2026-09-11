#include "ReflectionTags.h"

#include <ElevateEngine/Core/AssetRegistry.h>
#include <ElevateEngine/Core/AssetMetaData.h>

Elevate::AssetTag::AssetTag(const AssetMetaData& meta)
{
	Meta = meta;
#ifdef EE_EDITOR_BUILD
	AssetRegistry::RegisterAssetType(meta);
#endif
}
