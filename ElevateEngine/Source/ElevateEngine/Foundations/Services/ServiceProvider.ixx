module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

export module Elevate.Foundations.Services:ServiceProvider;

import Elevate.Foundations.CoreLogger;

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

		~ServiceProvider()
		{
			Shutdown();
		}

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
				CoreLogger::Error("Service not registered: {}", typeid(TService).name());
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
					m_singletonCreationOrder.push_back(type);
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

		void Shutdown()
		{
			if (m_isShutdown)
			{
				return;
			}

			m_isShutdown = true;

			for (auto it = m_singletonCreationOrder.rbegin(); it != m_singletonCreationOrder.rend(); ++it)
			{
				if (auto found = m_singletons.find(*it); found != m_singletons.end())
				{
					found->second->Shutdown();
					m_singletons.erase(found);
				}
			}
		}

	private:
		bool m_isShutdown = false;

		std::unordered_map<std::type_index, Internal::ServiceDescriptor> m_descriptors;

		std::unordered_map<std::type_index, std::unique_ptr<Internal::IServiceInstance>> m_singletons;
		std::vector<std::type_index> m_singletonCreationOrder;
	};
}
