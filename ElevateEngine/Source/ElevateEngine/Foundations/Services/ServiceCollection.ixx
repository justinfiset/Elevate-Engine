module;

#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <memory>
#include <new>
#include <utility>
#include <typeinfo>
#include <tuple>
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

		ServiceCollection(const ServiceCollection &) = delete;
		ServiceCollection &operator=(const ServiceCollection &) = delete;
		ServiceCollection(ServiceCollection &&) = default;
		ServiceCollection &operator=(ServiceCollection &&) = default;

		template <typename TService, typename TImplementation, typename... TDependencies>
		void RegisterSingleton()
		{
			AddServiceDescriptor<TService, TImplementation, TDependencies...>(Internal::ServiceLifetime::Singleton);
		}

		template <typename TService, typename TFactory>
		void RegisterSingleton(TFactory &&factory)
		{
			AddServiceDescriptorFromFactory<TService>(Internal::ServiceLifetime::Singleton, std::forward<TFactory>(factory));
		}

		template <typename TService, typename TImplementation, typename... TDependencies>
		void RegisterTransient()
		{
			AddServiceDescriptor<TService, TImplementation, TDependencies...>(Internal::ServiceLifetime::Transient);
		}

		template <typename TService, typename TFactory>
		void RegisterTransient(TFactory &&factory)
		{
			AddServiceDescriptorFromFactory<TService>(Internal::ServiceLifetime::Transient, std::forward<TFactory>(factory));
		}

		template <typename TService, typename TImplementation, typename... TDependencies>
		void RegisterScoped()
		{
			AddServiceDescriptor<TService, TImplementation, TDependencies...>(Internal::ServiceLifetime::Scoped);
		}

		template <typename TService, typename TFactory>
		void RegisterScoped(TFactory &&factory)
		{
			AddServiceDescriptorFromFactory<TService>(Internal::ServiceLifetime::Scoped, std::forward<TFactory>(factory));
		}

		ServiceProvider Build() &&;

	private:
		template <typename TService, typename TImplementation, typename... TDependencies>
		std::unique_ptr<Internal::IServiceFactory> CreateFactory()
		{
			// Create a factory that constructs instances of TImplementation
			return std::make_unique<Internal::ServiceFactory<TService, TImplementation>>(
				[](ServiceProvider &provider)
				{
					return std::make_shared<TImplementation>(provider.Resolve<TDependencies>()...);
				});
		}

		template <typename TService, typename TImplementation>
		void AddDescriptor(Internal::ServiceLifetime lifetime, std::unique_ptr<Internal::IServiceFactory> factory)
		{
			// Ensure that TImplementation is derived from TService
			static_assert(std::is_convertible_v<TImplementation *, TService *>);

			// Create a service descriptor and add it to the collection
			Internal::ServiceDescriptor descriptor(typeid(TService), lifetime, std::move(factory));

			auto result = m_serviceDescriptors.try_emplace(
				std::type_index(typeid(TService)),
				std::move(descriptor));

			if (!result.second)
			{
				CoreLogger::Warn("ServiceCollection : Service type already registered: {}. While trying to register {}", std::string(typeid(TService).name()), std::string(typeid(TImplementation).name()));
			}
		}

		template <typename TService, typename TImplementation, typename... TDependencies>
		void AddServiceDescriptor(Internal::ServiceLifetime lifetime)
		{
			AddDescriptor<TService, TImplementation>(
				lifetime,
				CreateFactory<TService, TImplementation, TDependencies...>());
		}

		template <typename TService, typename TFactory>
		void AddServiceDescriptorFromFactory(Internal::ServiceLifetime lifetime, TFactory &&factory)
		{
			using Result = std::invoke_result_t<TFactory &, ServiceProvider &>;
			static_assert(std::is_same_v<Result, std::shared_ptr<typename Result::element_type>>,
						  "The factory must return a std::shared_ptr<T>");

			using TImplementation = typename Result::element_type;

			AddDescriptor<TService, TImplementation>(
				lifetime,
				std::make_unique<Internal::ServiceFactory<TService, TImplementation>>(
					std::forward<TFactory>(factory)));
		}

		// Internal storage for service descriptors
		std::unordered_map<std::type_index, Internal::ServiceDescriptor> m_serviceDescriptors;
	};
}
