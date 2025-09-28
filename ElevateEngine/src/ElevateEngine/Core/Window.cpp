#include "eepch.h"
#include "Window.h"

#include <ElevateEngine/Core/Log.h>

// TODO use our custom loader instead of repetitive code herre
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
Elevate::WindowProps::WindowProps(const std::string appConfigFilePath)
{
	// TODO put this part in a util somehwerre to get the document
///////////////////////////////////////////////////////
	FILE* fp = fopen(appConfigFilePath.c_str(), "r");
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer,
		sizeof(readBuffer));
	rapidjson::Document doc;
	doc.ParseStream(is);
	fclose(fp);

	// Error handling
	if (doc.HasParseError())
	{
		EE_CORE_TRACE("ERROR PARSING JSON");
	}
	///////////////////////////////////////////////////////

	if (doc.HasMember("title") && doc["title"].IsString())
	{
		this->Title = doc["title"].GetString();
	}

	if (doc.HasMember("size"))
	{
		const rapidjson::Value& size = doc["size"];
		if (size.HasMember("x") && size["x"].IsInt())
		{
			this->Width = size["x"].GetInt();
		}
		if (size.HasMember("y") && size["y"].IsInt())
		{
			this->Height = size["y"].GetInt();
		}
	}

	if (doc.HasMember("vsync") && doc["vsync"].IsBool())
	{
		this->VSync = doc["vsync"].GetBool();
	}
}
