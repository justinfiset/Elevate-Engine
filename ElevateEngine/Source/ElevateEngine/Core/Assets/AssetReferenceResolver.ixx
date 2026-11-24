module;


#include <memory>

export module Elevate.Core.Assets.AssetReferenceResolver;

import Elevate.Foundations.Guid;
import Elevate.Core.Objects.Object;

export namespace Elevate::Detail
{
	std::shared_ptr<EEObject> ResolveAssetReference(const Guid& guid);
}
