#include "PathResolver.h"

#include <ElevateEngine/Core/Core.h>
#include <ElevateEngine/Core/Log.h>

std::string Elevate::PathResolver::Resolve(const std::string& virtualPath)
{
	if (virtualPath.starts_with("engine://"))
	{
		return std::string(EE_ENGINE_DIR) + virtualPath.substr(9);
	}
	else if (virtualPath.starts_with("editor://"))
	{
#ifndef EE_EDITOR_BUILD
		EE_CORE_ERROR("You cannot use editor:// path while building without the editor.");
		return virtualPath;
#else
		return std::string(EE_EDITOR_DIR) + virtualPath.substr(9);
#endif
	}
	else if (virtualPath.starts_with("content://"))
	{
		return std::string(EE_CONTENT_ROOT) + virtualPath.substr(10);
	}
	return virtualPath;
}