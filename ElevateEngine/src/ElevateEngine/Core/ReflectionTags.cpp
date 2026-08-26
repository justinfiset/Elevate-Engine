#include "ReflectionTags.h"

#include <ElevateEngine/Core/AssetRegistry.h>

Elevate::AssetTag::AssetTag(const AssetMetaData& meta)
{
	Meta = meta;
#ifdef EE_EDITOR_BUILD
	AssetRegistry::RegisterAssetType(meta);
#endif
}
