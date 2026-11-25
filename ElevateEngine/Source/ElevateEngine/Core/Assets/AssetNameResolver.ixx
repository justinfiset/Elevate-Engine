module;

#include <string>

export module Elevate.Core.Assets.AssetNameResolver;

import Elevate.Foundations.Guid;

export namespace Elevate::Detail
{
	std::string ResolveAssetName(const Guid& guid, const std::string& explicitName, const std::string& fallbackName);
}
