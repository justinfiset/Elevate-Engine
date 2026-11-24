module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Core.Assets.AssetTagRegistration;

import Elevate.Core.Assets.AssetMetadata;

export namespace Elevate::Detail
{
	void RegisterAssetTypeForTag(const AssetMetadata& meta);
}
