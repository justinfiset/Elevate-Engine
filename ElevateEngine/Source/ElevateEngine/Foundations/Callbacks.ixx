module;

#include <cstdint>

export module Elevate.Foundations.Callbacks;

export namespace Elevate
{
	struct CallbackID
	{
		uint64_t Value = 0;

		CallbackID() = default;

		CallbackID(uint64_t value)
			: Value(value)
		{
		}

		// Prefix: ++id
		CallbackID& operator++()
		{
			++Value;
			return *this;
		}

		// Postfix: id++
		CallbackID operator++(int)
		{
			CallbackID previous = *this;
			++(*this);
			return previous;
		}

		bool operator==(const CallbackID&) const = default;
	};
}