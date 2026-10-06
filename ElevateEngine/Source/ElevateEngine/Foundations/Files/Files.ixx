module;

#include <string>

export module Elevate.Foundations.Files;

export namespace Elevate::Files
{
	std::string GetFileContent(std::string path);
}