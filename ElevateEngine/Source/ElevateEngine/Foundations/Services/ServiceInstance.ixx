module;

#include <memory>
#include <utility>

export module Elevate.Foundations.Services:ServiceInstance;

import Elevate.Foundations.Cleanups;

export namespace Elevate::Internal
{
	class IServiceInstance
	{
	public:
		virtual ~IServiceInstance() = default;
		virtual void Shutdown() = 0;
	};

	template <typename TService>
	class ServiceInstance final : public IServiceInstance
	{
	public:
		explicit ServiceInstance(std::shared_ptr<TService> instance) : m_instance(std::move(instance))
		{
		}

		~ServiceInstance() override = default;

		void Shutdown() override
		{
			if constexpr (std::is_base_of_v<IShutdownable, TService>)
			{
				m_instance->Shutdown();
			}
		}

		std::shared_ptr<TService> Get() const
		{
			return m_instance;
		}

	private:
		std::shared_ptr<TService> m_instance;
	};
}
