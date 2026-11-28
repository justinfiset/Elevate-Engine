module;

#include <string>

module Elevate.Core.Assets.AssetNameResolver;

import Elevate.Foundations.Guid;
import Elevate.Core.Assets.AssetRegistry;

namespace Elevate::Detail
{
	std::string ResolveAssetName(const Guid& guid, const std::string& explicitName, const std::string& fallbackName)
	{
		if (!explicitName.empty())
		{
			return explicitName;
		}

		const AssetEntry* entry = AssetRegistry::GetEntry(guid);
		if (entry)
		{
			return entry->AssetName;
		}

		return fallbackName;
	}
}
