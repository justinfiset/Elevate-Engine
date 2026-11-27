module;

module Elevate.Core.Assets.AssetTagRegistration;

import Elevate.Core.Assets.AssetMetadata;
import Elevate.Core.Assets.AssetRegistry;

namespace Elevate::Detail
{
	void RegisterAssetTypeForTag(const AssetMetadata& metadata)
	{
		AssetRegistry::RegisterAssetType(metadata);
	}
}
