#pragma once
#include <type_traits>
#include <vector>

import Elevate.Core.Types.TypeField;

namespace Elevate::Internal
{
	template<typename T, typename = void>
	struct ScopeSelector
	{
		static std::vector<TypeField>& GetStack(T*)
		{
			return T::generated_structEntry.StructFieldStack;
		}
	};

	template<typename T>
	struct ScopeSelector<T, std::void_t<decltype(T::generated_classEntry)>>
	{
		static std::vector<TypeField>& GetStack(T*)
		{
			return T::generated_classEntry.ClassFieldStack;
		}
	};
}
