module;

#include <stdio.h>
#include <format>
#include <functional>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/allocators.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>

module Elevate.Core.Window;

import Elevate.Foundations.CoreLogger;
import Elevate.Core.Events.ApplicationEvent;
import Elevate.Core.Events.Event;

namespace Elevate
{
	WindowProps::WindowProps(const std::string appConfigFilePath)
	{
		// TODO put this part in a util somehwerre to get the document
		// todo : use impl in foundations to read file content;
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
			CoreLogger::Trace("ERROR PARSING The WindowProps JSON");
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

	Window* Window::Create(const WindowProps& props)
	{
		return CreatePlatformWindow(props);
	}

	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.FramebufferWidth = props.Width;
		m_Data.FramebufferHeight = props.Height;
		CoreLogger::Trace("Creating window: {} ({}x{})", props.Title.c_str(), props.Width, props.Height);
	}

	void Window::SetWindowSize(unsigned int width, unsigned int height)
	{
		CoreLogger::Info("Setting window size to {}x{}", width, height);
		m_Data.Width = width;
		m_Data.Height = height;
		WindowResizeEvent event(width, height);
		EventCallback(event);
	}
}
