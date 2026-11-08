module;

#include <string>

module Elevate.Core.Assets.Asset;

import Elevate.Core.Assets.AssetRegistry;

namespace Elevate
{
	std::string Asset::GetName() const
	{
		auto entry = AssetRegistry::GetEntry(m_guid);
		if (entry)
		{
			return entry->AssetName;
		}
		return generated_classEntry.ClassName;
	}
}