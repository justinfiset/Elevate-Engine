module Elevate.Core.Reflection.ReflectionTags;

import Elevate.Core.Assets.AssetRegistry;
import Elevate.Core.Assets.AssetMetadata;

namespace Elevate
{
	AssetTag::AssetTag(const AssetMetadata &meta)
	{
		Meta = meta;
#ifdef EE_EDITOR_BUILD
		AssetRegistry::RegisterAssetType(meta);
#endif
	}
}
