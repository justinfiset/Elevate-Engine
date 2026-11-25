module;

#include <memory>

module Elevate.Core.Assets.AssetReferenceResolver;

import Elevate.Foundations.Guid;
import Elevate.Core.Objects.Object;
import Elevate.Core.Assets.Asset;
import Elevate.Core.Assets.AssetRegistry;

namespace Elevate::Detail
{
	std::shared_ptr<EEObject> ResolveAssetReference(const Guid& guid)
	{
		return std::dynamic_pointer_cast<EEObject>(AssetRegistry::GetAsset<Asset>(guid));
	}
}
