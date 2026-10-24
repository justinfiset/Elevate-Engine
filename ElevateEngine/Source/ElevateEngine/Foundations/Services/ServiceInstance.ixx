module;

#include <memory>
#include <utility>

export module Elevate.Foundations.Services:ServiceInstance;

export namespace Elevate::Internal
{
	class IServiceInstance
	{
	public:
		virtual ~IServiceInstance() = default;
	};

	template <typename TService>
	class ServiceInstance final : public IServiceInstance
	{
	public:
		explicit ServiceInstance(std::shared_ptr<TService> instance) : m_instance(std::move(instance))
		{
		}

		~ServiceInstance() override = default;

		std::shared_ptr<TService> Get() const
		{
			return m_instance;
		}

	private:
		std::shared_ptr<TService> m_instance;
	};
}
