module;

#include <memory>
#include <vector>
#include <type_traits>
#include <utility>
#include <ranges>

export module Elevate.Foundations.Services:ServiceModuleRegistry;

import :IServiceModule;
import :ServiceCollection;

export namespace Elevate
{
	class ServiceModuleRegistry
	{
	public:
		static ServiceModuleRegistry Create()
		{
			return ServiceModuleRegistry();
		}

		template <typename T>
		ServiceModuleRegistry& Register()
		{
			static_assert(std::is_base_of<IServiceModule, T>::value, "T must derive from IServiceModule");

			auto module = std::make_unique<T>();
			m_modules.push_back(std::move(module));

			return *this;
		}

		/** @brief Builds the service module registry by registering all modules with the provided service collection.
		 *  @param services The service collection to register the modules with.
		 *  @return The built service module registry.
		 */
		ServiceModuleRegistry Build(ServiceCollection& services)
		{
			for (auto& module : m_modules)
			{
				module->Register(services);
			}

			return std::move(*this);
		}

		/** @brief Configures all registered service modules.
		 *  @param provider The service provider to configure the modules with.
		 */
		void Configure(ServiceProvider& provider)
		{
			for (auto& module : m_modules)
			{
				module->Configure(provider);
			}
		}

	private:
		ServiceModuleRegistry() = default;

		std::vector<std::unique_ptr<IServiceModule>> m_modules;
	};
}