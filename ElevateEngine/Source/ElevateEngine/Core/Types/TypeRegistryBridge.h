#pragma once

#include <cstddef>
#include <span>
#include <typeindex>

namespace Elevate::Internal
{
	const std::type_index* FindTypeIndex(const char* key);
	std::span<const std::byte> GetReflectedFieldData(const std::type_index& type);
}
