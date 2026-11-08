module;

#include <string>

export module Elevate.Foundations.Files;

export import Elevate.Foundations.Files.FileDialog;

export namespace Elevate::Files
{
	std::string GetFileContent(std::string path);
}