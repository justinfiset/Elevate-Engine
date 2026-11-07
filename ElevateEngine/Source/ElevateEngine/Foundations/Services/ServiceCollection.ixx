module;

#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <memory>
#include <utility>
#include <typeinfo>
#include <string>

export module Elevate.Foundations.Services:ServiceCollection;

import Elevate.Foundations.CoreLogger;

import :ServiceLifetime;
import :ServiceFactory;
import :ServiceDescriptor;
import :ServiceProvider;

export namespace Elevate
{
	class ServiceCollection
	{
	public:
		ServiceCollection() = default;
		~ServiceCollection() = default;

		ServiceCollection(const ServiceCollection&) = delete;
		ServiceCollection& operator=(const ServiceCollection&) = delete;
		ServiceCollection(ServiceCollection&&) = default;
		ServiceCollection& operator=(ServiceCollection&&) = default;

		template <typename TService, typename TImplementation, typename... TDependencies>
		void RegisterSingleton()
		{
			AddServiceDescriptor<TService, TImplementation, TDependencies...>(Internal::ServiceLifetime::Singleton);
		}

		template <typename TService, typename TImplementation, typename... TDependencies>
		void RegisterTransient()
		{
			AddServiceDescriptor<TService, TImplementation, TDependencies...>(Internal::ServiceLifetime::Transient);
		}

		template <typename TService, typename TImplementation, typename... TDependencies>
		void RegisterScoped()
		{
			AddServiceDescriptor<TService, TImplementation, TDependencies...>(Internal::ServiceLifetime::Scoped);
		}

		ServiceProvider Build()&&;

	private:
		template <typename TService, typename TImplementation, typename... TDependencies>
		std::unique_ptr<Internal::IServiceFactory> CreateFactory()
		{
			// Create a factory that constructs instances of TImplementation
			return std::make_unique<Internal::ServiceFactory<TService, TImplementation>>(
				[](ServiceProvider& provider)
				{
					return std::make_shared<TImplementation>(provider.Resolve<TDependencies>()...);
				});
		}

		template <typename TService, typename TImplementation, typename... TDependencies>
		void AddServiceDescriptor(Internal::ServiceLifetime lifetime)
		{
			// Ensure that TImplementation is derived from TService
			static_assert(std::is_convertible_v<TImplementation*, TService*>);

			// Create a factory for the implementation type
			auto factory = CreateFactory<TService, TImplementation, TDependencies...>();

			// Create a service descriptor and add it to the collection
			Internal::ServiceDescriptor descriptor(typeid(TService), lifetime, std::move(factory));

			auto [it, inserted] = m_serviceDescriptors.try_emplace(
				std::type_index(typeid(TService)),
				std::move(descriptor));

			if (!inserted)
			{
				CoreLogger::Warn("ServiceCollection : Service type already registered: {}. While trying to register {}", std::string(typeid(TService).name()), std::string(typeid(TImplementation).name()));
			}
		}

		// Internal storage for service descriptors
		std::unordered_map<std::type_index, Internal::ServiceDescriptor> m_serviceDescriptors;
	};
}
