
#include <ElevateEngine/Core/Types/TypeRegistryBridge.h>

import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeRegistry;

namespace Elevate::Internal
{
	const std::type_index* FindTypeIndex(const char* key)
	{
		auto& types = TypeRegistry::GetTypeIndexes();
		const auto it = types.find(key);

		if (it == types.end())
		{
			return nullptr;
		}

		return &it->second;
	}

	std::span<const std::byte> GetReflectedFieldData(const std::type_index& type)
	{
		auto& reflectedTypes = TypeRegistry::GetReflectedTypes();
		const auto it = reflectedTypes.find(type);

		if (it == reflectedTypes.end())
		{
			return {};
		}

		const auto& fields = it->second;
		return {reinterpret_cast<const std::byte*>(fields.data()), fields.size()};
	}
}
