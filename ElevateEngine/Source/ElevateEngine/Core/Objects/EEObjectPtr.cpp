module;

#include <memory>

module Elevate.Core.Objects.ObjectPtr;

import Elevate.Foundations.Guid;
import Elevate.Core.Objects.Object;
import Elevate.Core.Assets.AssetReferenceResolver;

namespace Elevate
{
	std::shared_ptr<EEObject> Detail::ResolveAssetHelper(const Guid& guid)
	{
		return Detail::ResolveAssetReference(guid);
	}
}
