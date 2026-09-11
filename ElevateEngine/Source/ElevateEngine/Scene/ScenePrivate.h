#pragma once

#include <unordered_map>
#include <memory>

#include <entt/entt.hpp>

namespace Elevate
{
	extern std::unordered_map<uint32_t, std::unique_ptr<entt::registry>>& GetRegistryMap();;
}