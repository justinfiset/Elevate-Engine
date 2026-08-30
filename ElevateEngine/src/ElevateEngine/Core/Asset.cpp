#include "Asset.h"

#include <ElevateEngine/Core/AssetRegistry.h>

namespace Elevate
{
	std::string Asset::GetName() const
	{
		auto entry = AssetRegistry::GetEntry(m_guid);
		if (entry)
		{
			return entry->AssetName;
		}
		return EEObject::GetName();
	}
}
