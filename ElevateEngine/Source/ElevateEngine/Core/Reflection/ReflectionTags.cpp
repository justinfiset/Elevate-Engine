module;


module Elevate.Core.Reflection.ReflectionTags;

import Elevate.Core.Assets.AssetMetadata;
import Elevate.Core.Assets.AssetTagRegistration;

namespace Elevate
{
	AssetTag::AssetTag(const AssetMetadata& meta)
	{
		Meta = meta;
#ifdef EE_EDITOR_BUILD
		Detail::RegisterAssetTypeForTag(meta);
#endif
	}
}
