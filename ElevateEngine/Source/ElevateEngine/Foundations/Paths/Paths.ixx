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
		static const std::filesystem::path Editor();
	};

	class PathResolver
	{
	public:
		static std::filesystem::path Resolve(std::string_view virtualPath);
	};
}