#pragma once
#include <string>

namespace Elevate
{
	class ISerializable
	{
	public:
		virtual std::string Serialize() const { return ""; }

		virtual void Deserialize(const std::string& data) { }
	};
}