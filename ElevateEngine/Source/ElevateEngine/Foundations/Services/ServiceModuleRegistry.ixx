module;

#include <memory>
#include <vector>
#include <type_traits>
#include <utility>

export module Elevate.Foundations.Services:ServiceModuleRegistry;

import :IServiceModule;
import :ServiceCollection;

export namespace Elevate
{
	class ServiceModuleRegistry
	{
	public:
		static ServiceModuleRegistry& Create()
		{
			return *new ServiceModuleRegistry();
		}

		template <typename T>
		ServiceModuleRegistry& Register()
		{
			static_assert(std::is_base_of<IServiceModule, T>::value, "T must derive from IServiceModule");

			auto module = std::make_unique<T>();
			m_modules.push_back(std::move(module));

			return *this;
		}

		void Build(ServiceCollection& services)
		{
			for (const auto& module : m_modules)
			{
				module->Register(services);
			}
		}

	private:
		ServiceModuleRegistry() = default;

		std::vector<std::unique_ptr<IServiceModule>> m_modules;
	};
}