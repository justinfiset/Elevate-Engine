module;

#include <memory>

export module Elevate.Foundations.Services:SharedService;

import :Fwd;
import :ServiceDescriptor;
import :ServiceInstance;

export namespace Elevate
{
    template <typename T>
    using SharedService = std::shared_ptr<T>;
}
