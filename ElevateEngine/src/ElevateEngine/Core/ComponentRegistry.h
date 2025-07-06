#pragma once

#include <typeindex>
#include <map>
#include <string>
#include <functional>
#include <entt/entt.hpp>

namespace Elevate {
    class Component;
}

namespace Elevate
{
    class ComponentRegistry {
    public:
        using ComponentFactory = std::function<Component* (entt::registry&, entt::entity)>;

        template<typename T>
        static void Register(const std::string& name) {
            GetFactories()[&typeid(T)] = [](entt::registry& registry, entt::entity entity) -> Component* {
                if (registry.all_of<T>(entity)) {
                    return static_cast<Component*>(&registry.get<T>(entity));
                }
                return nullptr;
            };

            GetTypeNames()[std::type_index(typeid(T))] = name;
        }

        static auto& GetFactories() {
            static std::unordered_map<const std::type_info*, ComponentFactory> factories;
            return factories;
        }

        static auto& GetTypeNames() {
            static std::unordered_map<std::type_index, std::string> names;
            return names;
        }

        static std::string GetName(const std::type_info& type) {
            auto& names = GetTypeNames();
            auto it = names.find(std::type_index(type));
            if (it != names.end()) {
                return it->second;
            }
            else {
                return type.name();
            }
        }
    };
}

#define REGISTER_COMPONENT(T) \
    namespace { \
        struct T##Registrar { \
            T##Registrar() { \
                Elevate::ComponentRegistry::Register<T>(#T); \
            } \
        } T##_registrar; \
    }