module;

#include <memory>
#include <typeindex>
#include <utility>

export module Elevate.Foundations.Services:ServiceDescriptor;

import :Fwd;
export import :ServiceLifetime;
export import :ServiceFactory;
import :ServiceInstance;

export namespace Elevate::Internal
{
	struct ServiceDescriptor
	{
	public:
		ServiceDescriptor(std::type_index serviceType, ServiceLifetime lifetime, std::unique_ptr<IServiceFactory> factory)
			: m_serviceType(serviceType), m_lifetime(lifetime), m_factory(std::move(factory))
		{
		}

		std::type_index GetServiceType() const
		{
			return m_serviceType;
		}

		ServiceLifetime GetLifetime() const
		{
			return m_lifetime;
		}

		std::unique_ptr<IServiceInstance> Create(ServiceProvider &provider) const
		{
			return m_factory->Create(provider);
		}

	private:
		std::type_index m_serviceType;
		ServiceLifetime m_lifetime;
		std::unique_ptr<IServiceFactory> m_factory;
	};
}