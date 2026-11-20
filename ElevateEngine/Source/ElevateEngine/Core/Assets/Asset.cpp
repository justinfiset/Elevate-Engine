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

		if (!m_name.empty())
		{
			return m_name;
		}

		return generated_classEntry.ClassName;
	}

	void Asset::SetName(const std::string& name)
	{
		m_name = name;
	}
}