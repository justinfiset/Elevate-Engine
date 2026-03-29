#pragma once

#include <string>

namespace Elevate
{
	class PathResolver
	{
	public:
		static std::string Resolve(const std::string& virtualPath);
	};
}