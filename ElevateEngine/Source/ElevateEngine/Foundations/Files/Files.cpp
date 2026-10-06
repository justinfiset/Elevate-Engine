module;

#include <fstream>
#include <string>

module Elevate.Foundations.Files;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Paths;

namespace Elevate::Files
{
	std::string GetFileContent(std::string path)
	{
		auto resolvedPath = PathResolver::Resolve(path);

		std::string content;
		std::ifstream s(resolvedPath, std::ios::in);

		// If file does not exists
		if (!s.is_open())
		{
			CoreLogger::Error("Could not open file : {0}, file does not exist", resolvedPath.string());
			return std::string();
		}

		// Get all of the lines from the file
		std::string line = "";
		while (!s.eof())
		{
			std::getline(s, line);
			content.append(line + "\n");
		}

		// Close the file (good practice)
		s.close();

		return content;
	}
}