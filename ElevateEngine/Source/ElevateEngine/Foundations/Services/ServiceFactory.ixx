module;

#include <memory>
#include <functional>
#include <utility>

export module Elevate.Foundations.Services:ServiceFactory;

import :Fwd;
import :ServiceInstance;

export namespace Elevate::Internal
{
	class IServiceFactory
	{
	public:
		virtual ~IServiceFactory() = default;

		virtual std::unique_ptr<IServiceInstance> Create(ServiceProvider& provider) = 0;
	};

	template <typename TService, typename TImplementation>
	class ServiceFactory : public IServiceFactory
	{
	public:
		using FactoryFunction = std::function<std::shared_ptr<TImplementation>(ServiceProvider&)>;

		explicit ServiceFactory(FactoryFunction factory) : m_factory(std::move(factory))
		{
		}

		std::unique_ptr<IServiceInstance> Create(ServiceProvider& provider) override
		{
			// Create the service
			std::shared_ptr<TService> service = m_factory(provider);
			// Create the service instance
			return std::make_unique<ServiceInstance<TService>>(std::move(service));
		}

	private:
		FactoryFunction m_factory;
	};
}
