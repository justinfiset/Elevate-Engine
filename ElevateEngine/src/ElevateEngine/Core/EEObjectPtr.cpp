#include "EEObjectPtr.h"

#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/AssetRegistry.h>

namespace Elevate
{
	std::shared_ptr<EEObject> Detail::ResolveAssetHelper(const Guid& guid)
	{
		return AssetRegistry::GetAsset<Asset>(guid);
	}
}