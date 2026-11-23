#include <ElevateEngine/Core/Types/TypeRegistryBridge.h>

import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeRegistry;

namespace Elevate::Internal
{
	const std::type_index* FindTypeIndex(const void* key)
	{
		auto& types = TypeRegistry::GetTypeIndexes();
		const auto it = types.find(key);

		if (it == types.end())
		{
			return nullptr;
		}

		return &it->second;
	}

	const void* GetReflectedFieldData(const std::type_index& type, std::size_t& count)
	{
		auto& reflectedTypes = TypeRegistry::GetReflectedTypes();
		const auto it = reflectedTypes.find(type);

		if (it == reflectedTypes.end())
		{
			count = 0;
			return nullptr;
		}

		count = it->second.size();
		return it->second.data();
	}
}
