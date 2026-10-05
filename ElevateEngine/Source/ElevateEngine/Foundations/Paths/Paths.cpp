module;

#include <filesystem>
#include <string_view>

module Elevate.Foundations.Paths;

import Elevate.Foundations.CoreLogger;

namespace Elevate
{
	const std::filesystem::path Paths::Engine()
	{
#ifndef EE_PLATFORM_WEB
		return EE_RESOURCE_DIR "/Engine/";
#else
		return EE_ENGINE_DIR "/Engine/";
#endif
	}

	const std::filesystem::path Paths::Content()
	{
		return "./Content/";
	}

#ifdef EE_EDITOR_BUILD
	const std::filesystem::path Paths::Editor()
	{
#ifndef EE_PLATFORM_WEB
		return EE_RESOURCE_DIR "/Editor/";
#else
		return EE_EDITOR_DIR "/Editor/";
#endif
	}
#endif

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
#ifndef EE_EDITOR_BUILD
			CoreLogger::Error("You cannot use editor:// path while building without the editor.");
			return virtualPath;
#else
			return Paths::Editor() / virtualPath.substr(9);
#endif
		}
		return virtualPath;
	}
}