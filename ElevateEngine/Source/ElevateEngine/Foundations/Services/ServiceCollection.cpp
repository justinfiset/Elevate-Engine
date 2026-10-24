module;

#include <utility>

module Elevate.Foundations.Services;

namespace Elevate
{
	ServiceProvider ServiceCollection::Build()&&
	{
		return ServiceProvider(std::move(m_serviceDescriptors));
	}
}
