#pragma once
#include <string>
#include <ctime>
#include <cstdint>
#include <memory>

namespace Elevate
{
	class Texture;
}

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

	struct ProjectTemplate
	{
		std::string Name = "";
		std::string Path = "";
		std::string Desription = "";
		std::shared_ptr<Elevate::Texture> Thumbnail = nullptr;
	};
}