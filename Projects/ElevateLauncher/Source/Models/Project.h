#pragma once
#include <string>
#include <ctime>
#include <cstdint>

namespace EL
{
	struct Project
	{
		bool IsValid = false;
		uint32_t Id = 0;
		std::string Name = "";
		std::string Path = "";
		std::time_t LastOpened = 0;

		bool UsesWwise = false;
	};
}