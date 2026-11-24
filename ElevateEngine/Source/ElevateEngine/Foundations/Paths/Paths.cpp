module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <filesystem>
#include <string_view>

module Elevate.Foundations.Paths;

import Elevate.Foundations.CoreLogger;

namespace Elevate
{
	const std::filesystem::path Paths::Engine()
	{
		return EE_RESOURCE_DIR "/Engine/";
	}

	const std::filesystem::path Paths::Content()
	{
		return "./Content/";
	}

	const std::filesystem::path Paths::Editor()
	{
		return EE_RESOURCE_DIR "/Editor/";
	}

	std::filesystem::path PathResolver::Resolve(std::string_view virtualPath)
	{
		if (virtualPath.starts_with("engine://"))
		{
			return Paths::Engine() / virtualPath.substr(9);
		}
		else if (virtualPath.starts_with("content://"))
		{
			return Paths::Content() / virtualPath.substr(10);
		}
		else if (virtualPath.starts_with("editor://"))
		{
			return Paths::Editor() / virtualPath.substr(9);
		}
		return virtualPath;
	}
}