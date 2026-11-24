module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <string>

module Elevate.Core.Assets.Asset;

import Elevate.Core.Assets.AssetNameResolver;

namespace Elevate
{
	std::string Asset::GetName() const
	{
		return Detail::ResolveAssetName(m_guid, m_explicitName, generated_classEntry.ClassName);
	}

	void Asset::SetName(const std::string& name)
	{
		m_explicitName = name;
	}
}
