#pragma once
#include <ElevateEngine/Core/TypeRegistry.h>

namespace Elevate::Internal
{
    template<typename T, typename = void>
    struct ScopeSelector
    {
        static std::vector<::Elevate::TypeField>& GetStack(T*)
        {
            return T::generated_structEntry.StructFieldStack;
        }
    };

    template<typename T>
    struct ScopeSelector<T, std::void_t<decltype(T::generated_classEntry)>>
    {
        static std::vector<::Elevate::TypeField>& GetStack(T*)
        {
            return T::generated_classEntry.ClassFieldStack;
        }
    };
}
