#include "eepch.h"
#include "FileUtility.h"

#include <fstream>
#include <ElevateEngine/Core/Log.h>

std::string Elevate::File::GetFileContent(std::string path)
{
	std::string content;
	std::ifstream s(path, std::ios::in);

	// If file does not exists
	if (!s.is_open())
	{
		EE_CORE_ERROR("Could not open file : {0}, file does not exist", path);
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
