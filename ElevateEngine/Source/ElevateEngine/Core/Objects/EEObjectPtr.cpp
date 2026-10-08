module;

#include <memory>

module Elevate.Core.Objects.ObjectPtr;

import Elevate.Core.Assets.AssetRegistry;

// Forwards
namespace Elevate
{
	class EEObject;
	class Guid;
}

namespace Elevate
{
	std::shared_ptr<EEObject> Detail::ResolveAssetHelper(const Guid& guid)
	{
		return AssetRegistry::GetAsset<Asset>(guid);
	}
}