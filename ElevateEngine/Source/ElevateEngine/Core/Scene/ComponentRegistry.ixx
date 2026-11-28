module;

#include <cstdint>
#include <memory>
#include <entt/entity/registry.hpp>

export module Elevate.Core.Scene:ComponentRegistry;

// EnTT lives in the module's global module fragment. Export using-declarations
// for the names used by the ComponentRegistry templates and their dependent
// ADL, without re-exporting the whole header.
namespace entt
{
	export using ::entt::entity;
	export using ::entt::basic_any;
	export using ::entt::any_cast;
	export using ::entt::basic_registry;
	export using ::entt::registry;
}

namespace entt::internal
{
	export using ::entt::internal::operator-;
	export using ::entt::internal::operator==;
	export using ::entt::internal::operator!=;
	export using ::entt::internal::operator<;
	export using ::entt::internal::operator>;
	export using ::entt::internal::operator<=;
	export using ::entt::internal::operator>=;
}

export namespace Elevate
{
	class ComponentRegistry
	{
	public:
		static constexpr uint32_t InvalidEntity = UINT32_MAX;

		ComponentRegistry();
		~ComponentRegistry();
		ComponentRegistry(ComponentRegistry&&) noexcept;
		ComponentRegistry& operator=(ComponentRegistry&&) noexcept;
		ComponentRegistry(const ComponentRegistry&) = delete;
		ComponentRegistry& operator=(const ComponentRegistry&) = delete;

		/////////////////////////////////////////////
		// Registry
		/////////////////////////////////////////////
		void CreateRegistry(uint32_t registryId);
		void DestroyRegistry(uint32_t registryId);
		bool HasRegistry(uint32_t registryId) const;

		/////////////////////////////////////////////
		// Entity
		/////////////////////////////////////////////

		uint32_t CreateEntity(uint32_t registryId);
		void DestroyEntity(uint32_t registryId, uint32_t entityId);

		template<typename T>
		T* TryGet(uint32_t registryId, uint32_t entityId)
		{
			auto* reg = Native(registryId);
			return reg ? reg->try_get<T>(entt::entity(entityId)) : nullptr;
		}

		template<typename T, typename... Args>
		T& Add(uint32_t registryId, uint32_t entityId, Args&&... args)
		{
			return Native(registryId)->emplace<T>(entt::entity(entityId), std::forward<Args>(args)...);
		}

		template<typename T>
		bool Has(uint32_t registryId, uint32_t entityId)
		{
			auto* reg = Native(registryId);
			return reg && reg->all_of<T>(entt::entity(entityId));
		}

		template<typename T>
		void Remove(uint32_t registryId, uint32_t entityId)
		{
			if (auto* reg = Native(registryId))
			{
				reg->remove<T>(entt::entity(entityId));
			}
		}

		/////////////////////////////////////////////
		// Context
		/////////////////////////////////////////////

		template<typename T, typename... Args>
		T& EmplaceContext(uint32_t registryId, Args&&... args)
		{
			return Native(registryId)->ctx().emplace<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		T* TryGetContext(uint32_t registryId)
		{
			auto* reg = Native(registryId);
			return reg ? reg->ctx().find<T>() : nullptr;
		}

		template<typename T>
		bool HasContext(uint32_t registryId)
		{
			auto* reg = Native(registryId);
			return reg && reg->ctx().contains<T>();
		}

	private:
		entt::registry* Native(uint32_t registryId);

		struct Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}
