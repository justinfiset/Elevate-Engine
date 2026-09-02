#include "EEObjectPtr.h"

#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Core/AssetRegistry.h>

// Forwards
namespace Elevate
{
	class Guid;
}

namespace Elevate
{
	std::shared_ptr<EEObject> Detail::ResolveAssetHelper(const Guid& guid)
	{
		return AssetRegistry::GetAsset<Asset>(guid);
	}
}