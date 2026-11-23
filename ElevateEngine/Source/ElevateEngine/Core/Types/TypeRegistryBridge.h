#pragma once

#include <cstddef>
#include <typeindex>

namespace Elevate::Internal
{
	const std::type_index* FindTypeIndex(const void* key);
	const void* GetReflectedFieldData(const std::type_index& type, std::size_t& count);
}
