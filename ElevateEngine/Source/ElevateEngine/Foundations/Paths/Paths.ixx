module;

#include <filesystem>
#include <string_view>

export module Elevate.Foundations.Paths;

export namespace Elevate
{
	class Paths
	{
	public:
		static const std::filesystem::path Engine();
		static const std::filesystem::path Content();
#ifdef EE_EDITOR_BUILD
		static const std::filesystem::path Editor();
#endif
	};

	class PathResolver
	{
	public:
		static std::filesystem::path Resolve(std::string_view virtualPath);
	};
}