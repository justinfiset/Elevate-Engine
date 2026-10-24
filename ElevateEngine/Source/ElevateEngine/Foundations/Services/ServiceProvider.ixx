module;

#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

export module Elevate.Foundations.Services:ServiceProvider;

import :Fwd;
import :ServiceDescriptor;
import :ServiceInstance;

export namespace Elevate
{
	class ServiceProvider
	{
	public:
		explicit ServiceProvider(std::unordered_map<std::type_index, Internal::ServiceDescriptor> descriptors)
			: m_descriptors(std::move(descriptors))
		{
		}

		~ServiceProvider() = default;

		ServiceProvider(const ServiceProvider&) = delete;
		ServiceProvider& operator=(const ServiceProvider&) = delete;
		ServiceProvider(ServiceProvider&&) = default;
		ServiceProvider& operator=(ServiceProvider&&) = default;

		template <typename TService>
		std::shared_ptr<TService> Resolve()
		{
			const std::type_index type(typeid(TService));

			auto descriptorIt = m_descriptors.find(type);
			if (descriptorIt == m_descriptors.end())
			{
				return nullptr;
			}

			const Internal::ServiceDescriptor& descriptor = descriptorIt->second;
			switch (descriptor.GetLifetime())
			{
			case Internal::ServiceLifetime::Singleton:
			{
				auto it = m_singletons.find(type);
				if (it == m_singletons.end())
				{
					auto instance = descriptor.Create(*this);
					it = m_singletons.emplace(type, std::move(instance)).first;
				}
				return static_cast<Internal::ServiceInstance<TService> &>(*it->second).Get();
			}
			case Internal::ServiceLifetime::Transient:
			{
				auto instance = descriptor.Create(*this);
				return static_cast<Internal::ServiceInstance<TService> &>(*instance).Get();
			}
			case Internal::ServiceLifetime::Scoped:
			{
				// todo: impl.
				break;
			}
			}

			return nullptr;
		}

		template <typename TService>
		bool IsRegistered()
		{
			return m_descriptors.find(std::type_index(typeid(TService))) != m_descriptors.end();
		}

	private:
		std::unordered_map<std::type_index, Internal::ServiceDescriptor> m_descriptors;
		std::unordered_map<std::type_index, std::unique_ptr<Internal::IServiceInstance>> m_singletons;
	};
}
