#pragma once
#include <string>
#include <ctime>
#include <cstdint>
#include <memory>

#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
	class Texture;
}

namespace EL
{
	struct Project
	{
		BEGIN_STRUCT(Project)
		bool IsValid = false;
		PROPERTY(IsValid)
		uint32_t Id = 0;
		PROPERTY(Id)
		std::string Name = "";
		PROPERTY(Name)
		std::string Path = "";
		PROPERTY(Path)
		std::time_t LastOpened = 0;
		PROPERTY(LastOpened)
		bool UsesWwise = false;
		PROPERTY(UsesWwise)
		END_STRUCT()
	};

	struct ProjectList
	{
		BEGIN_STRUCT(ProjectList)
		std::vector<Project> Projects;
		PROPERTY(Projects);
		END_STRUCT()
	};

	struct ProjectTemplate
	{
		std::string Name = "";
		std::string Path = "";
		std::string Desription = "";
		std::shared_ptr<Elevate::Texture> Thumbnail = nullptr;
	};
}