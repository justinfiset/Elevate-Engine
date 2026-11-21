module;

#include <string>

module Elevate.Core.Assets.Asset;

import Elevate.Core.Assets.AssetRegistry;

namespace Elevate
{
	std::string Asset::GetName() const
	{
		if (!m_explicitName.empty())
		{
			return m_explicitName;
		}

		auto entry = AssetRegistry::GetEntry(m_guid);
		if (entry)
		{
			return entry->AssetName;
		}

		return generated_classEntry.ClassName;
	}

	void Asset::SetName(const std::string& name)
	{
		m_explicitName = name;
	}
}