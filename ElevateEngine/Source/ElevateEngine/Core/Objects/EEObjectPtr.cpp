module;

#include <memory>

module Elevate.Core.Objects.ObjectPtr;

import Elevate.Core.Objects.Object;
import Elevate.Core.Assets.Asset;
import Elevate.Core.Assets.AssetRegistry;

// Forwards
namespace Elevate
{
	class Guid;
}

namespace Elevate
{
	std::shared_ptr<EEObject> Detail::ResolveAssetHelper(const Guid& guid)
	{
		return std::dynamic_pointer_cast<EEObject>(AssetRegistry::GetAsset<Asset>(guid));
	}
}