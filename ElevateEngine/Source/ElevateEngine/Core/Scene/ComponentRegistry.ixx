module;

#include <cstdint>
#include <memory>
#include <entt/entt.hpp>

export module Elevate.Core.Scene:ComponentRegistry;

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

	private:
		entt::registry* Native(uint32_t registryId);

		struct Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}
