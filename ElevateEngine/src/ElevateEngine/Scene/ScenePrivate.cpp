#include "eepch.h"
#include "ScenePrivate.h"

namespace Elevate
{
    std::unordered_map<uint32_t, std::unique_ptr<entt::registry>>& GetRegistryMap() {
        static std::unordered_map<uint32_t, std::unique_ptr<entt::registry>> s_RegistryMap;
        return s_RegistryMap;
    }
}